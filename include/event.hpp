#pragma once

#include "messages.hpp"

const size_t event_size = 128;

struct event
{
public:
    char* buffer();

    header header();

    template< typename T >
    T body();

    size_t size();

    void zero();

    event& operator=( event& e );

private:
    char buf_[ event_size ];
};

std::ostream& operator<<( std::ostream& o, const event& e );

#include "event.inl"
