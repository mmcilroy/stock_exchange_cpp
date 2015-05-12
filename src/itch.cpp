#include <boost/asio.hpp>
#include <map>
#include <iostream>

using boost::asio::ip::tcp;

typedef unsigned long long io_session_id;

struct io_event {};

template< typename H >
struct io_session;


// ----------------------------------------------------------------------------
template< typename H >
class io_controller
{
public:
    void accept( int port );

    io_session_id connect( int port );

    void write( io_session_id, const io_event& );

    void close( io_session_id );

//private:
    void on_read( io_session_id, const io_event& );

    void on_close( io_session_id );

    boost::asio::io_service io_;

    std::map< io_session_id, io_session< H >* > sessions_;

    H handler_;
};



// ----------------------------------------------------------------------------
template< typename H >
struct io_session : public std::enable_shared_from_this< io_session< H > >
{
    io_session( io_controller< H >& );

    ~io_session();

    void read();

//private:
    io_controller< H >& ioc_;

    io_session_id id_;
};



// ----------------------------------------------------------------------------
template< typename H >
void io_controller< H >::accept( int port )
{
    std::function< void( tcp::acceptor&, tcp::socket& ) > accept_again =
        [ this, accept_again ]( tcp::acceptor& acceptor, tcp::socket& socket )
        {
            acceptor.async_accept( socket,
                [&]( boost::system::error_code ec )
                {
                    if( !ec )
                    {
                        auto sess = std::make_shared< io_session< H > >( *this );
                        accept_again( acceptor, socket );
                    }
                } );
        };

    tcp::acceptor acceptor( io_, tcp::endpoint( tcp::v4(), port ) );
    tcp::socket socket( io_ );
    accept_again( acceptor, socket );
    io_.run();
}



// ----------------------------------------------------------------------------
template< typename H >
io_session_id io_controller< H >::connect( int port )
{
    return 1;
}

template< typename H >
void io_controller< H >::write( io_session_id, const io_event& )
{
}

template< typename H >
void io_controller< H >::close( io_session_id )
{
}

template< typename H >
void io_controller< H >::on_read( io_session_id id, const io_event& ev )
{
    handler_.on_read( id, ev );
}

template< typename H >
void io_controller< H >::on_close( io_session_id id )
{
    sessions_.erase( id );
    handler_.on_close( id );
}



// ----------------------------------------------------------------------------
template< typename H >
io_session< H >::io_session( io_controller< H >& ioc ) : ioc_( ioc )
{
}

template< typename H >
io_session< H >::~io_session()
{
    ioc_.on_close( id_ );
}

template< typename H >
void io_session< H >::read()
{
    io_event e;
    ioc_.on_read( id_, e );
}



// ----------------------------------------------------------------------------
int main()
{
    struct io_handler
    {
        void on_read( io_session_id, const io_event& ) {};
        void on_close( io_session_id ) {};
    };

    io_controller< io_handler > ioc;
    ioc.accept( 14002 );
}





#if 0

#include "events.hpp"

#include <boost/asio.hpp>
#include <map>
#include <iostream>

using boost::asio::ip::tcp;

typedef unsigned long long io_session_id;

class io_session;



class io_controller
{
public:
    template< typename R, typename C >
    void accept( int port, R on_read, C on_close );

    template< typename R, typename C >
    io_session_id connect( int port, R on_read, C on_close );

    void write( io_session_id sess, const packed_event& event );

    void closed( io_session_id sess );

    io_session_id get_next_id();

private:
    template< typename R, typename C >
    void accept( tcp::acceptor& acceptor, tcp::socket& socket, R on_read, C on_close );

    std::map< io_session_id, io_session* > sessions_;

    boost::asio::io_service io_;

    io_session_id next_id_ = 0;
};



class io_session : public std::enable_shared_from_this< io_session >
{
public:
    io_session( io_controller& ioc, tcp::socket socket ) :
        ioc_( ioc ),
        id_( ioc.get_next_id() ),
        socket_( std::move( socket ) )
    {
    }

    template< typename R, typename C >
    void read( R on_read, C on_close )
    {
        auto self( shared_from_this() );
        boost::asio::async_read(
            socket_,
            boost::asio::buffer( event_.get_header_buf(), packed_header::size ),
            boost::asio::transfer_exactly( packed_header::size ),
            [ this, self, on_read, on_close ]( boost::system::error_code ec, std::size_t length )
            {
                if( ec )
                {
                    closed( on_close );
                }
                else
                {
                    header hdr;
                    unpack( event_.get_header_buf(), 0, hdr );

                    boost::asio::async_read(
                        socket_,
                        boost::asio::buffer( event_.get_payload_buf(), hdr.length_ ),
                        boost::asio::transfer_exactly( hdr.length_ ),
                        [ this, self, on_read, on_close ]( boost::system::error_code ec, std::size_t length )
                        {
                            if( ec )
                            {
                                closed( on_close );
                            }
                            else
                            {
                                on_read( id_, event_ );
                                read( on_read, on_close );
                            }
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

    template< typename C >
    void closed( C on_close )
    {
        on_close( id_ );
        ioc_.closed( id_ );
    }

    io_session_id get_id() const
    {
        return id_;
    }

private:
    tcp::socket socket_;

    io_controller& ioc_;
    io_session_id id_;
    packed_event event_;
};




template< typename R, typename C >
void io_controller::accept( int port, R on_read, C on_close )
{
    tcp::acceptor acceptor( io_, tcp::endpoint( tcp::v4(), port ) );
    tcp::socket socket( io_ );
    accept( acceptor, socket, on_read, on_close );
    io_.run();
}

template< typename R, typename C >
io_session_id io_controller::connect( int port, R on_read, C on_close )
{
    return 1;
}

void io_controller::write( io_session_id sess, const packed_event& )
{
}

void io_controller::closed( io_session_id sess )
{
    sessions_.erase( sess );
}

template< typename R, typename C >
void io_controller::accept( tcp::acceptor& acceptor, tcp::socket& socket, R on_read, C on_close )
{
    acceptor.async_accept( socket,
        [&]( boost::system::error_code ec )
        {
            if( !ec )
            {
                auto sess = std::make_shared< io_session >( *this, std::move( socket ) );
                sess->read( on_read, on_close );
                sessions_[ sess->get_id() ] = sess.get();
            }
            accept( acceptor, socket, on_read, on_close );
        } );
}

io_session_id io_controller::get_next_id()
{
    return next_id_++;
}


void acceptor_thread( io_controller* ioc )
{
    ioc->accept(
        14002,
        []( io_session_id sess, const packed_event& ev ) {
            std::cout << sess << " " << ev << std::endl;
        },
        []( io_session_id sess ) {
            std::cout << sess << " closed" << std::endl;
        }
    );
}

void connector_thread( io_controller* ioc )
{
    ioc->connect( 14002,
        []( io_session_id sess, const packed_event& ev ) {
            std::cout << sess << " " << ev << std::endl;
        },
        []( io_session_id sess ) {
            std::cout << sess << " closed" << std::endl;
        }
    );
}

int main()
{
    io_controller ioc;

    std::thread io_thread( acceptor_thread, &ioc );
    io_thread.join();
}

#endif





#if 0

#include "packed_events.hpp"

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

    void write( const packed_packed_event& packed_event )
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
    packed_packed_event event_;
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

    void write( io_session_id id, const packed_packed_event& packed_event )
    {
        auto sess = sessions_.find( id );
        if( sess != sessions_.end() )
        {
            io_.dispatch( [ sess, packed_event ]() {
                sess->second->write( packed_event );
            } );
        }
    }

    void close( io_session_id id )
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

    void run()
    {
        io_.run();
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
                else
                {
                    std::cout << "err " << ec.value() << std::endl;
                }
                accept( acceptor, socket, handler );
            } );
    }

    boost::asio::io_service io_;

    std::map< io_session_id, std::shared_ptr< io_session > > sessions_;

    io_session_id next_id_ = 0;
};

int main()
{
    io_acceptor ioa;
    ioa.accept( []( io_session& sess, packed_packed_event& packed_event ) {
        std::cout << packed_event << std::endl;
    } );
}

#endif
