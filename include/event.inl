/*
inline event::event()
{
}

inline event::event( const event& )
{
    std::cout << "111\n";
}
*/

inline size_t event::size()
{
    return header::size() + get_header().length();
}

inline void event::zero()
{
    memset( buf_, 0, event_size );
}

inline char* event::get_header_buf()
{
    return buf_;
}

inline header event::get_header()
{
    return header( buf_ );
}

inline void event::set_header( header h )
{
    memcpy( buf_, h.buffer(), h.size() );
}

inline char* event::get_body_buf()
{
    return buf_ + header::size();
}

template< typename T >
inline T event::get_body()
{
    return T( buf_ + header::size() );
}

template< typename T >
inline void event::set_body( T b )
{
    memcpy( buf_ + header::size(), b.buffer(), b.size() );
}

inline event& event::operator=( event& e )
{
    memcpy( buf_, e.buf_, e.size() );
    return *this;
}
