using boost::asio::ip::tcp;

struct io_session : public std::enable_shared_from_this< io_session >
{
    io_session( io_controller&, io_session_id id, tcp::socket );

    ~io_session();

    template< typename H >
    void read( H handler );

private:
    io_controller& ioc_;
    io_session_id id_;
    io_event event_;

    tcp::socket socket_;
};

io_session::io_session( io_controller& ioc, io_session_id id, tcp::socket socket ) :
    ioc_( ioc ),
    id_( id ),
    socket_( std::move( socket ) )
{
}

io_session::~io_session()
{
    ioc_.closed( id_ );
}

template< typename H >
void io_session::read( H handler )
{
    std::cout << "start read" << std::endl;

    auto self( shared_from_this() );

    boost::asio::async_read(
        socket_,
        boost::asio::buffer( event_.header_buffer(), buffer_size< header >() ),
        boost::asio::transfer_exactly( buffer_size< header >() ),
        [ this, self, handler ]( boost::system::error_code ec, std::size_t length )
        {
            std::cout << "read " << length << std::endl;

            if( !ec )
            {
                header hdr;
                event_.unpack( hdr );

                std::cout << hdr << std::endl;

                boost::asio::async_read(
                    socket_,
                    boost::asio::buffer( event_.payload_buffer(), hdr.size_ ),
                    boost::asio::transfer_exactly( hdr.size_ ),
                    [ this, self, handler ]( boost::system::error_code ec, std::size_t length )
                    {
                        std::cout << "read " << length << std::endl;

                        if( !ec )
                        {
                            handler( event_ );
                            read( handler );
                        }
                        else
                        {
                            std::cout << ec.message() << std::endl;
                        }
                    } );
            }
            else
            {
                std::cout << ec.message() << std::endl;
            }
        } );
}

template< typename H >
void io_controller::accept( int port, H handler )
{
    std::function< void( tcp::acceptor&, tcp::socket& ) > accept_again =
        [&]( tcp::acceptor& acceptor, tcp::socket& socket )
        {
            acceptor.async_accept( socket,
                [&]( boost::system::error_code ec )
                {
                    if( !ec )
                    {
                        auto sess = std::make_shared< io_session >( *this, 1, std::move( socket ) );
                        sess->read( handler );
                        accept_again( acceptor, socket );
                    }
                } );
        };

    tcp::acceptor acceptor( io_, tcp::endpoint( tcp::v4(), port ) );
    tcp::socket socket( io_ );
    accept_again( acceptor, socket );
    io_.run();
}

inline void io_controller::write( const io_event& )
{
    // post write
}

inline void io_controller::closed( io_session_id id )
{
    std::cout << "closed " << id << std::endl;
}
