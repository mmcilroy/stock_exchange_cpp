#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

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

    char buf_[ size ];
};

#include "events.inl"
