#include "io_controller.hpp"
#include <thread>

void io_thread( io_controller* ioc )
{
    ioc->connect( 14002, []( const io_event& e ) {
        ;
    } );
}

int main( int argc, char* argv[] )
{
    io_controller ioc;
    std::thread io_thr( io_thread, &ioc );

    {
        io_event e;

        session s;
        s.session_id_ = 0;
        e.pack( s );

        login l;
        strcpy( l.user_id_, "test" );
        e.pack( l );

        ioc.write( e );
    }

    {
        io_event e;

        session s;
        s.session_id_ = 0;
        e.pack( s );

        place_order po;
        po.transaction_id_ = 4567;
        po.parameters_.type_ = order_type_t::limit;
        po.parameters_.side_ = order_side_t::buy;
        po.parameters_.quantity_ = 100;
        po.parameters_.price_ = 100;
        strcpy( po.parameters_.symbol_, "test" );
        e.pack( po );

        ioc.write( e );
    }

    io_thr.join();

    return 0;
}
