#pragma once

#include "events.hpp"
#include "publisher.hpp"

typedef publisher< packed_event, blocking_sequence > event_publisher;
typedef subscriber< packed_event, blocking_sequence > event_subscriber;

class io
{
public:
    io();

    ~io();

    void accept( uint16_t port, event_publisher* inp );

    //void send( const event& );

private:
    class impl; std::unique_ptr< impl > impl_;
};
