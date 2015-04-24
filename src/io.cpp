#include "io.hpp"
#include "event.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const size_t body_size = 256;

class session : public std::enable_shared_from_this< session >
{
public:
    session( tcp::socket socket, event_publisher* inp ) :
        socket_( std::move( socket ) ),
        inp_( inp ),
        hdr_( event_.get_header() ),
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

        event_.zero();
        boost::asio::async_read(
            socket_,
            boost::asio::buffer( event_.get_header_buf(), header::size() ),
            boost::asio::transfer_exactly( header::size() ),
            [ this, self ]( boost::system::error_code ec, std::size_t length ) {
                if( !ec ) {
                    read_body();
                }
            } );
    }

    void read_body()
    {
        auto self( shared_from_this() );

        std::cout << this << " " << hdr_ << std::endl;

        boost::asio::async_read(
            socket_,
            boost::asio::buffer( event_.get_body_buf(), hdr_.length() ),
            boost::asio::transfer_exactly( hdr_.length() ),
            [ this, self ]( boost::system::error_code ec, std::size_t length ) {
                if( !ec ) {
                    process();
                }
            } );
    }

    void process()
    {
        if( logged_on_ )
        {
            if( hdr_.type() == place_order::id() )
            {
                place_order p = event_.get_body< place_order >();

                std::cout << this << " " << p << std::endl;

                inp_->publish( 1, [ this ]( event& e ) {
                    e = event_;
                } );
            }
        }
        else
        {
            if( hdr_.type() == login::id() )
            {
                login l = event_.get_body< login >();
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
    event event_;
    header hdr_;

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
