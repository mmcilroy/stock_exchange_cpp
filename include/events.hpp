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

template< size_t N >
struct packed_buffer
{
    static constexpr size_t size = N;

    char buf_[ size ];
};

#include "types.hpp"
#include "events.inl"

union packed_payloads
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

struct packed_event
{
    static constexpr size_t size = sizeof(packed_header) + sizeof(packed_payloads);

    char buf_[ size ];

    char* get_header_buf()
    {
        return buf_;
    }

    const char* get_header_buf() const
    {
        return buf_;
    }

    char* get_payload_buf()
    {
        return buf_ + sizeof(packed_header);
    }

    const char* get_payload_buf() const
    {
        return buf_ + sizeof(packed_header);
    }

    size_t length() const
    {
        header hdr;
        ::unpack( buf_, 0, hdr );
        return sizeof(packed_header) + hdr.length_;
    }

    template< typename T >
    T& pack( T& target )
    {
        header hdr;
        hdr.type_ = T::id;
        hdr.length_ = T::size;
        ::pack( buf_, 0, hdr );
        ::pack( buf_ + sizeof(packed_header), 0, target );
        return target;
    }

    template< typename T >
    T& unpack( T& target ) const
    {
        ::unpack( buf_ + sizeof(packed_header), 0, target );
        return target;
    }
};

template<>
inline header& packed_event::unpack< header >( header& target ) const
{
    ::unpack( buf_, 0, target );
    return target;
}

inline std::ostream& operator<<( std::ostream& out, const packed_event& in )
{
    header hdr;
    out << in.unpack( hdr );

    if( hdr.type_ == place_order::id ) {
        place_order po; out << " | " << in.unpack( po );
    } else if( hdr.type_ == order_placed::id ) {
        order_placed op; out << " | " << in.unpack( op );
    } else if( hdr.type_ == order_executed::id ) {
        order_executed oe; out << " | " << in.unpack( oe );
    } else if( hdr.type_ == login::id ) {
        login l; out << " | " << in.unpack( l );
    } else {
        out << " | ???";
    }

    return out;
};

#include "publisher.hpp"

typedef publisher< packed_event, blocking_sequence > event_publisher;
typedef subscriber< packed_event, blocking_sequence > event_subscriber;
