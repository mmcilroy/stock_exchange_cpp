#pragma once

#include <cstring>
#include <iostream>

template< typename T >
size_t pack( void* buffer, size_t offset, const T* source, size_t length=1 )
{
    memcpy( (char*)buffer+offset, source, sizeof(T)*length );
    return offset+sizeof(T)*length;
}

template< typename T >
size_t unpack( const void* buffer, size_t offset, T* target, size_t length=1 )
{
    memcpy( target, (char*)buffer+offset, sizeof(T)*length );
    return offset+sizeof(T)*length;
}

template< typename T, size_t N >
struct packed_buffer
{
    static constexpr size_t size = N;

    void pack( const T& source )
    {
        pack( buf_, 0, source );
    }

    void unpack( T& target ) const
    {
        unpack( buf_, 0, target );
    }

    packed_buffer& operator=( const packed_buffer& other )
    {
        if( this != &other ) {
            memcpy( buf_, other.buf_, size );
        }
        return *this;
    }

    char buf_[ size ];
};

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

#include "events.inl"

#include "publisher.hpp"

union all_payloads
{
    packed_login login_;
    packed_order_parameters order_parameters_;
    packed_order_rejected order_rejected_;
    packed_order_placed order_placed_;
    packed_order_pulled order_pulled_;
    packed_order_executed order_executed_;
    packed_place_order place_order_;
    packed_pull_order pull_order_;
};

struct packed_payload
{
    static constexpr size_t size = packed_header::size + sizeof(all_payloads);    

    char buf_[ size ];
};

struct event
{
    packed_header header_;
    packed_payload payload_;
};

typedef publisher< event, blocking_sequence > event_publisher;

typedef subscriber< event, blocking_sequence > event_subscriber;
