#pragma once

#include "structs.hpp"

// payload exists only to give us a way of knowing the maximum buffer
// size required for a payload at compile time
union payload
{
    char session_[ buffer_size< session >() ];
    char header_[ buffer_size< header >() ];
    char login_[ buffer_size< login >() ];
    char order_parameters_[ buffer_size< order_parameters >() ];
    char order_rejected_[ buffer_size< order_rejected >() ];
    char order_placed_[ buffer_size< order_placed >() ];
    char order_pulled_[ buffer_size< order_pulled >() ];
    char order_executed_[ buffer_size< order_executed >() ];
    char place_order_[ buffer_size< place_order >() ];
    char pull_order_[ buffer_size< pull_order >() ];
};

template<>
constexpr size_t buffer_size< payload >()
{
    return sizeof(payload);
}

class io_event
{
public:
    static constexpr size_t max_size = 
        buffer_size< session >() + buffer_size< header >() + buffer_size< payload >();

    template< typename T >
    T& pack( T& target );

    template< typename T >
    T& unpack( T& target ) const;

    void* session_buffer();

    const void* session_buffer() const;

    void* header_buffer();

    const void* header_buffer() const;

    void* payload_buffer();

    const void* payload_buffer() const;

    size_t size() const;

private:
    char buf_[ max_size ];
};

#include "io_event.inl"
