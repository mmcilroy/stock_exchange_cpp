#include "io.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this< session >
{
public:
    session( tcp::socket socket, event_publisher* ev_pub ) :
        socket_( std::move( socket ) ),
        ev_pub_( ev_pub ),
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
            boost::asio::buffer( ev_.get_header_buf(), packed_header::size ),
            boost::asio::transfer_exactly( packed_header::size ),
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
        unpack( ev_.get_header_buf(), 0, hdr );

        boost::asio::async_read(
            socket_,
            boost::asio::buffer( ev_.get_payload_buf(), hdr.length_ ),
            boost::asio::transfer_exactly( hdr.length_ ),
            [ this, self, hdr ]( boost::system::error_code ec, std::size_t length ) {
                if( !ec ) {
                    process( hdr.type_ );
                }
            } );
    }

    void process( uint16_t type )
    {
        std::cout << "< " << ev_ << std::endl;

        if( logged_on_ )
        {
            ev_pub_->publish( 1, [ this ]( packed_event& ev, size_t i ) {
                ev = ev_;
            } );
        }
        else
        {
            if( type == login::id )
            {
                login l;
                unpack( ev_.get_payload_buf(), 0, l );
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

    event_publisher* ev_pub_;
    packed_event ev_;

    bool logged_on_;
};

struct io::impl
{
    void accept( uint16_t port, event_publisher* ev_pub )
    {
        ev_pub_ = ev_pub;
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
                    std::make_shared< session >( std::move( s ), ev_pub_ )->read_header();
                }
                accept( a, s );
            } );
    }

    boost::asio::io_service io_;
    event_publisher* ev_pub_;
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
