#pragma once

typedef uint64_t exchange_id_t;
typedef uint32_t order_quantity_t;
typedef uint32_t order_price_t;

enum class order_type_t { limit, market };
enum class order_side_t { buy, sell };

inline std::ostream& operator<<( std::ostream& out, const order_type_t& in )
{
    if( in == order_type_t::limit ) {
        out << "limit";
    } else if( in == order_type_t::market ) {
        out << "market";
    }
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_side_t& in )
{
    if( in == order_side_t::buy ) {
        out << "buy";
    } else if( in == order_side_t::sell ) {
        out << "sell";
    }
    return out;
};
