#include "io_journal.hpp"

int main()
{
    {
        io_journal ioj( "out", true );
        io_event e;

        header h;
        session s;
        s.session_id_ = 0;
        e.pack( s );

        {
            // message 1
            h.sequence_ = 1;
            e.pack( h );

            login l;
            strcpy( l.user_id_, "one" );
            e.pack( l );

            ioj.write( e );
        }

        {
            // message 2
            h.sequence_ = 2;
            e.pack( h );

            place_order po;
            po.transaction_id_ = 4567;
            po.parameters_.type_ = order_type_t::limit;
            po.parameters_.side_ = order_side_t::buy;
            po.parameters_.quantity_ = 100;
            po.parameters_.price_ = 100;
            strcpy( po.parameters_.symbol_, "test" );
            e.pack( po );

            ioj.write( e );
        }

        {
            // message 3
            h.sequence_ = 3;
            e.pack( h );

            login l;
            strcpy( l.user_id_, "two" );
            e.pack( l );

            ioj.write( e );
        }
    }

    {
        io_journal ioj( "out", true );
        ioj.read( 0, 1, 3, []( const io_event& e ) {
            std::cout << e << std::endl;
        } );
    }
}
