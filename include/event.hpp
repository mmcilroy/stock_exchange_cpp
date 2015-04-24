#pragma once

#include "messages.hpp"

const size_t event_size = 128;

struct event
{
public:
    char* get_header_buf();

    header get_header();

    void set_header( header h );

    char* get_body_buf();

    template< typename T >
    T get_body();

    template< typename T >
    void set_body( T b );

    event& operator=( event& e );

    size_t size();

    void zero();

private:
    char buf_[ event_size ];
};

std::ostream& operator<<( std::ostream& o, const event& e );

#include "event.inl"
