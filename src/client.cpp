// g++ -std=c++11 -I../include client.cpp messages.cpp -lboost_system -lpthread -o client

#include "event.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main( int argc, char* argv[] )
{
    boost::asio::io_service io;
    tcp::socket socket( io );
    tcp::resolver resolver( io );

    boost::asio::connect( socket, resolver.resolve( { "localhost", "14002" } ) ); 

    event e;
    header h = e.get_header();

    {
        e.zero();
        h.type( login::id() );
        h.length( login::size() );

        login l = e.get_body< login >();
        l.user_id( 1234 );

        std::cout << e << std::endl;

        boost::asio::write( socket, boost::asio::buffer( e.get_header_buf(), e.size() ) );
    }

    {
        e.zero();
        h.type( place_order::id() );
        h.length( place_order::size() );

        place_order o = e.get_body< place_order >();
        o.user_id( 1234 );
        o.transaction_id( 4567 );

        order_parameters p = o.parameters();
        p.quantity( 100 );
        p.price( 100 );
        p.side( 1 );

        std::cout << e << std::endl;

        boost::asio::write( socket, boost::asio::buffer( e.get_header_buf(), e.size() ) );
    }

    return 0;
}
