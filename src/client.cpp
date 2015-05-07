// g++ -std=c++11 -I../include client.cpp messages.cpp -lboost_system -lpthread -o client

#include "events.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this< session >
{
public:
    session( tcp::socket socket ) :
        socket_( std::move( socket ) )
    {
    }

    void read_header()
    {
        auto self( shared_from_this() );

        boost::asio::async_read(
            socket_,
            boost::asio::buffer( ev_.get_header_buf(), packed_header::size ),
            boost::asio::transfer_exactly( packed_header::size ),
            [ this, self ]( boost::system::error_code ec, size_t length ) {
                if( !ec ) {
                    read_payload();
                }
            } );
    }

    void read_payload()
    {
        auto self( shared_from_this() );

        header hdr;
        ev_.unpack( hdr );

        boost::asio::async_read(
            socket_,
            boost::asio::buffer( ev_.get_payload_buf(), hdr.length_ ),
            boost::asio::transfer_exactly( hdr.length_ ),
            [ this, self ]( boost::system::error_code ec, size_t length ) {
                if( !ec ) {
                    std::cout << ev_ << std::endl;
                    read_header();
                }
            } );
    }

    void write( const packed_event& ev )
    {
        std::cout << "> " << ev << std::endl;
        boost::asio::write(
            socket_,
            boost::asio::buffer( ev.buf_, ev.length() ) );
    }

    tcp::socket socket_;

    packed_event ev_;
};

void io_thread( boost::asio::io_service* io )
{
    io->run();
}

int main( int argc, char* argv[] )
{
    std::cout << sizeof(packed_event) << std::endl;

    boost::asio::io_service io;
    tcp::socket socket( io );
    tcp::resolver resolver( io );

    boost::asio::connect( socket, resolver.resolve( { "localhost", "14002" } ) ); 

    auto sess = std::make_shared< session >( std::move( socket ) );
    sess->read_header();

    std::thread io_thr( io_thread, &io );

    packed_event ev;

    {
        login l;
        strcpy( l.user_id_, "test" );
        ev.pack( l );
        io.post( [ &sess, ev ]() { 
            sess->write( ev );
        } );

    }

    {
        place_order po;
        po.user_id_ = 1234;
        po.transaction_id_ = 4567;
        strcpy( po.parameters_.symbol_, "test" );
        po.parameters_.type_ = order_type_t::limit;
        po.parameters_.side_ = order_side_t::buy;
        po.parameters_.quantity_ = 100;
        po.parameters_.price_ = 100;
        ev.pack( po );
        io.post( [ &sess, ev ]() { 
            sess->write( ev );
        } );
    }

    io_thr.join();






    /*
    {
        packed_header ph;
        header h;
        h.type_ = login::id;
        h.length_ = packed_login::size;
        std::cout << h << std::endl;
        pack( ph.buf_, 0, h );
        boost::asio::write( socket, boost::asio::buffer( ph.buf_, ph.size ) );
    }

    {
        packed_login pl;
        login l;
        strcpy( l.user_id_, "test" );
        std::cout << l << std::endl;
        pack( pl.buf_, 0, l );
        boost::asio::write( socket, boost::asio::buffer( pl.buf_, pl.size ) );
    }

    {
        packed_header ph;
        header h;
        h.type_ = place_order::id;
        h.length_ = packed_place_order::size;
        std::cout << h << std::endl;
        pack( ph.buf_, 0, h );
        boost::asio::write( socket, boost::asio::buffer( ph.buf_, ph.size ) );
    }

    {
        packed_place_order ppo;
        place_order po;
        po.user_id_ = 1234;
        po.transaction_id_ = 4567;
        strcpy( po.parameters_.symbol_, "test" );
        po.parameters_.type_ = order_type_t::limit;
        po.parameters_.side_ = order_side_t::buy;
        po.parameters_.quantity_ = 100;
        po.parameters_.price_ = 100;
        std::cout << po << std::endl;
        pack( ppo.buf_, 0, po );
        boost::asio::write( socket, boost::asio::buffer( ppo.buf_, ppo.size ) );
    }
    */

    return 0;
}
