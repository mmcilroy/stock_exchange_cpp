#pragma once

#include "event.hpp"
#include "publisher.hpp"

typedef publisher< event, blocking_sequence > event_publisher;
typedef subscriber< event, blocking_sequence > event_subscriber;

class io
{
public:
    io();

    ~io();

    void accept( uint16_t port, event_publisher* inp );

    void send( const event& );

private:
    class impl; std::unique_ptr< impl > impl_;
};
