#include "event.hpp"

std::ostream& operator<<( std::ostream& o, const event& ce )
{
    event& e = (event&)ce;

    o << "header: " << e.get_header() << ", body: ";
    switch( e.get_header().type() )
    {
        case login::id():
            o << e.get_body< login >(); break;

        case place_order::id():
            o << e.get_body< place_order >(); break;

        default:
            o << "???";
    }

    return o;
}
