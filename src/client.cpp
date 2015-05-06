// g++ -std=c++11 -I../include client.cpp messages.cpp -lboost_system -lpthread -o client

#include "events.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main( int argc, char* argv[] )
{
    boost::asio::io_service io;
    tcp::socket socket( io );
    tcp::resolver resolver( io );

    boost::asio::connect( socket, resolver.resolve( { "localhost", "14002" } ) ); 

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

    return 0;
}
