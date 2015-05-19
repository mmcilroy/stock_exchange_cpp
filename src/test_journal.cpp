#include "io_journal.hpp"

int main()
{
    {
        io_journal ioj( "out", true );

        io_event e;

        header h;
        h.sequence_ = 1;
        e.pack( h );

        session s;
        s.session_id_ = 0;
        e.pack( s );

        login l;
        strcpy( l.user_id_, "test" );
        e.pack( l );

        ioj.write( e );
    }

    {
        io_journal ioj( "out", true );
        ioj.read( 0, 1, 1, []( const io_event& e ) {
            std::cout << e << std::endl;
        } );
    }
}
