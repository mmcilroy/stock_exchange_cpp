#include "io.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this< session >
{
public:
    session( tcp::socket socket, event_publisher* inp ) :
        socket_( std::move( socket ) ),
        inp_( inp ),
        logged_on_( false )
    {
        std::cout << this << " new session" << std::endl;
    }

    ~session()
    {
        std::cout << this << " del session" << std::endl;
    }

    void read_header()
    {
        auto self( shared_from_this() );

        boost::asio::async_read(
            socket_,
            boost::asio::buffer( header_.buf_, header_.size ),
            boost::asio::transfer_exactly( header_.size ),
            [ this, self ]( boost::system::error_code ec, std::size_t length ) {
                if( !ec ) {
                    read_payload();
                }
            } );
    }

    void read_payload()
    {
        auto self( shared_from_this() );

        header hdr;
        unpack( header_.buf_, 0, hdr );

        boost::asio::async_read(
            socket_,
            boost::asio::buffer( payload_.buf_, hdr.length_ ),
            boost::asio::transfer_exactly( hdr.length_ ),
            [ this, self, hdr ]( boost::system::error_code ec, std::size_t length ) {
                if( !ec ) {
                    process( hdr.type_ );
                }
            } );
    }

    void process( uint16_t type )
    {
        if( logged_on_ )
        {
            inp_->publish( 1, [ this ]( event& e ) {
                e.header_ = header_;
                e.payload_ = payload_;
            } );
        }
        else
        {
            if( type == login::id )
            {
                login l;
                unpack( payload_.buf_, 0, l );
                logged_on_ = true;
            }
            else
            {
                std::cout << this << " login required" << std::endl;
            }
        }

        read_header();
    }

    tcp::socket socket_;

    event_publisher* inp_;

    packed_header header_;
    packed_payload payload_;

    bool logged_on_;
};

struct io::impl
{
    void accept( uint16_t port, event_publisher* inp )
    {
        inp_ = inp;
        tcp::acceptor acceptor( io_, tcp::endpoint( tcp::v4(), 14002 ) );
        tcp::socket socket( io_ );
        accept( acceptor, socket );
        io_.run();
    }

    void accept( tcp::acceptor& a, tcp::socket& s )
    {
        a.async_accept(
             s,
            [&]( boost::system::error_code ec ) {
                if( !ec ) {
                    std::make_shared< session >( std::move( s ), inp_ )->read_header();
                }
                accept( a, s );
            } );
    }

    boost::asio::io_service io_;
    event_publisher* inp_;
};

io::io() :
    impl_( new impl() )
{
}

io::~io()
{
}

void io::accept( uint16_t port, event_publisher* inp )
{
    impl_->accept( port, inp );
}
