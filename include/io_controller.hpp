#pragma once

#include "io_event.hpp"

#include <boost/asio.hpp>

typedef unsigned long long io_session_id;

class io_controller
{
public:
    friend class io_session;

    template< typename H >
    void accept( int port, H handler );

    void write( const io_event& );

private:
    void closed( io_session_id );

    boost::asio::io_service io_;
};

#include "io_controller.inl"
