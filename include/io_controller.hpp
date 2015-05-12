#pragma once

#include "io_event.hpp"

#include <boost/asio.hpp>

typedef unsigned long long io_session_id;

class io_session;

class io_controller
{
public:
    friend class io_session;

    template< typename H >
    void accept( int port, H handler );

    template< typename H >
    void connect( int port, H handler );

    void write( const io_event& );

private:
    void closed( io_session_id );

    io_session_id alloc_id();

    std::map< io_session_id, io_session* > sessions_;

    boost::asio::io_service io_;
};

#include "io_controller.inl"
