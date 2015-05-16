#pragma once

#include "io_journal.hpp"
#include "io_event.hpp"

#include <boost/asio.hpp>

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
    void closed( session_id_t );

    session_id_t alloc_id();

    std::map< session_id_t, io_session* > sessions_;

    boost::asio::io_service io_;
};

#include "io_controller.inl"
