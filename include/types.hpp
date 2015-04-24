#pragma once

#include <cstdint>
#include <ostream>

typedef uint64_t id;
typedef uint64_t quantity;
typedef uint64_t price;

enum class order_type { LIMIT, MARKET };
enum class order_state { NEW, PART_FILLED, FILLED, REJECTED, CANCELLED };
enum class side { BUY, SELL };

std::ostream& operator<<( std::ostream& o, const order_type& t )
{
    if( t == order_type::LIMIT ) {
        o << "LIMIT";
    } else {
        o << "MARKET";
    }
    return o;
};

std::ostream& operator<<( std::ostream& o, const side& s )
{
    if( s == side::BUY ) {
        o << "BUY";
    } else {
        o << "SELL";
    }
    return o;
};
