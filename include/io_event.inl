template< typename T >
T& io_event::pack( T& target )
{
    header hdr;
    hdr.type_ = payload_type< T >();
    hdr.size_ = buffer_size< T >();
    pack( hdr );
    ::pack( payload_buffer(), 0, target );
    return target;
}

template<>
header& io_event::pack< header >( header& target )
{
    ::pack( header_buffer(), 0, target );
    return target;
}

template<>
session& io_event::pack< session >( session& target )
{
    ::pack( session_buffer(), 0, target );
    return target;
}

template< typename T >
T& io_event::unpack( T& target ) const
{
    ::unpack( payload_buffer(), 0, target );
    return target;
}

template<>
header& io_event::unpack< header >( header& target ) const
{
    ::unpack( header_buffer(), 0, target );
    return target;
}

template<>
session& io_event::unpack< session >( session& target ) const
{
    ::unpack( buf_, 0, target );
    return target;
}

inline void* io_event::session_buffer()
{
    return buf_;
}

inline const void* io_event::session_buffer() const
{
    return buf_;
}

inline void* io_event::header_buffer()
{
    return buf_ + buffer_size< session >();
}

inline const void* io_event::header_buffer() const
{
    return buf_ + buffer_size< session >();
}

inline void* io_event::payload_buffer()
{
    return buf_ + buffer_size< session >() + buffer_size< header >();
}

inline const void* io_event::payload_buffer() const
{
    return buf_ + buffer_size< session >() + buffer_size< header >();
}

inline size_t io_event::size() const
{
    header hdr;
    unpack( hdr );
    return buffer_size< session >() + buffer_size< header >() + hdr.size_;
}

inline std::ostream& operator<<( std::ostream& out, const io_event& in )
{
    header hdr;
    out << in.unpack( hdr );

    if( hdr.type_ == payload_type< place_order >() ) {
        place_order po; out << " | " << in.unpack( po );
    } else if( hdr.type_ == payload_type< order_placed >() ) {
        order_placed op; out << " | " << in.unpack( op );
    } else if( hdr.type_ == payload_type< order_executed >() ) {
        order_executed oe; out << " | " << in.unpack( oe );
    } else if( hdr.type_ == payload_type< login >() ) {
        login l; out << " | " << in.unpack( l );
    } else {
        out << " | ???";
    }

    return out;
};
