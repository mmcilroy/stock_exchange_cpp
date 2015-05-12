#pragma once

#include "events.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class io_session : public std::enable_shared_from_this< io_session >
{
public:
    typedef uint64_t id;

    io_session( tcp::socket socket, id i ) :
        socket_( std::move( socket ) ),
        id_( i )
    {
        std::cout << "new session" << std::endl;
    }

    ~io_session()
    {
        std::cout << "del session" << std::endl;
    }

    template< typename F >
    void read( F handler )
    {
        auto self( shared_from_this() );
        boost::asio::async_read(
            socket_,
            boost::asio::buffer( event_.get_header_buf(), packed_header::size ),
            boost::asio::transfer_exactly( packed_header::size ),
            [ this, self, handler ]( boost::system::error_code ec, std::size_t length )
            {
                if( !ec )
                {
                    header hdr;
                    unpack( event_.get_header_buf(), 0, hdr );

                    boost::asio::async_read(
                        socket_,
                        boost::asio::buffer( event_.get_payload_buf(), hdr.length_ ),
                        boost::asio::transfer_exactly( hdr.length_ ),
                        [ this, self, handler ]( boost::system::error_code ec, std::size_t length )
                        {
                            if( !ec ) {
                                handler( *this, event_ );
                            }
                            read( handler );
                        } );
                }
            } );
    }

    void write( const packed_event& event )
    {
        boost::asio::write(
            socket_,
            boost::asio::buffer( event_.buf_, event_.length() ) );
    }

    void close()
    {
        socket_.close();
    }

    id get_id() const
    {
        return id_;
    }

private:
    tcp::socket socket_;
    packed_event event_;
    id id_;
};

class io_acceptor
{
public:
    template< typename F >
    void accept( F handler )
    {
        tcp::acceptor acceptor( io_, tcp::endpoint( tcp::v4(), 14002 ) );
        tcp::socket socket( io_ );
        accept( acceptor, socket, handler );
        io_.run();
    }

    void write( io_session::id id, const packed_event& event )
    {
        auto sess = sessions_.find( id );
        if( sess != sessions_.end() )
        {
            io_.dispatch( [ sess, event ]() {
                sess->second->write( event );
            } );
        }
    }

    void close( io_session::id id )
    {
        auto sess = sessions_.find( id );
        if( sess != sessions_.end() )
        {
            io_.dispatch( [ sess ]() {
                sess->second->close();
            } );
            sessions_.erase( sess );
        }
    }

private:
    template< typename F >
    void accept( tcp::acceptor& acceptor, tcp::socket& socket, F handler )
    {
        acceptor.async_accept( socket,
            [&]( boost::system::error_code ec )
            {
                if( !ec )
                {
                    auto sess = std::make_shared< io_session >( std::move( socket ), ++next_id_ );
                    sess->read( handler );
                    sessions_[ sess->get_id() ] = sess;
                }
                accept( acceptor, socket, handler );
            } );
    }

    boost::asio::io_service io_;

    std::map< io_session::id, std::shared_ptr< io_session > > sessions_;

    io_session::id next_id_ = 0;
};
