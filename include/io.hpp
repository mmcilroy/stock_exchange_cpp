#pragma once

#include "events.hpp"

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
