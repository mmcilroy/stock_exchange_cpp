struct session
{
    session_id_t session_id_;
};

struct header
{
    uint16_t size_;
    uint16_t type_;
    sequence_t sequence_;
};

struct login
{
    char user_id_[16];
};

struct resend
{
    sequence_t sequence_from_;
    sequence_t sequence_to_;
};

struct order_parameters
{
    char symbol_[8];
    order_type_t type_;
    order_side_t side_;
    order_quantity_t quantity_;
    order_price_t price_;
};

struct order_rejected
{
    exchange_id_t transaction_id_;
    order_parameters parameters_;
};

struct order_placed
{
    exchange_id_t transaction_id_;
    order_parameters parameters_;
};

struct order_pulled
{
    exchange_id_t transaction_id_;
    order_quantity_t leaves_;
    order_parameters parameters_;
};

struct order_executed
{
    exchange_id_t transaction_id_;
    order_price_t exec_price_;
    order_quantity_t exec_quantity_;
    order_quantity_t leaves_;
    order_parameters parameters_;
};

struct place_order
{
    exchange_id_t transaction_id_;
    order_parameters parameters_;
};

struct pull_order
{
    exchange_id_t transaction_id_;
};

template<>
constexpr size_t buffer_size<session>()
{
    return sizeof(session_id_t);
}

template<>
constexpr size_t payload_type<session>()
{
    return 0;
}

template<>
constexpr size_t buffer_size<header>()
{
    return sizeof(uint16_t) + sizeof(uint16_t) + sizeof(sequence_t);
}

template<>
constexpr size_t payload_type<header>()
{
    return 1;
}

template<>
constexpr size_t buffer_size<login>()
{
    return 16*sizeof(char);
}

template<>
constexpr size_t payload_type<login>()
{
    return 2;
}

template<>
constexpr size_t buffer_size<resend>()
{
    return sizeof(sequence_t) + sizeof(sequence_t);
}

template<>
constexpr size_t payload_type<resend>()
{
    return 3;
}

template<>
constexpr size_t buffer_size<order_parameters>()
{
    return 8*sizeof(char) + sizeof(order_type_t) + sizeof(order_side_t) + sizeof(order_quantity_t) + sizeof(order_price_t);
}

template<>
constexpr size_t payload_type<order_parameters>()
{
    return 4;
}

template<>
constexpr size_t buffer_size<order_rejected>()
{
    return sizeof(exchange_id_t) + buffer_size<order_parameters>();
}

template<>
constexpr size_t payload_type<order_rejected>()
{
    return 5;
}

template<>
constexpr size_t buffer_size<order_placed>()
{
    return sizeof(exchange_id_t) + buffer_size<order_parameters>();
}

template<>
constexpr size_t payload_type<order_placed>()
{
    return 6;
}

template<>
constexpr size_t buffer_size<order_pulled>()
{
    return sizeof(exchange_id_t) + sizeof(order_quantity_t) + buffer_size<order_parameters>();
}

template<>
constexpr size_t payload_type<order_pulled>()
{
    return 7;
}

template<>
constexpr size_t buffer_size<order_executed>()
{
    return sizeof(exchange_id_t) + sizeof(order_price_t) + sizeof(order_quantity_t) + sizeof(order_quantity_t) + buffer_size<order_parameters>();
}

template<>
constexpr size_t payload_type<order_executed>()
{
    return 8;
}

template<>
constexpr size_t buffer_size<place_order>()
{
    return sizeof(exchange_id_t) + buffer_size<order_parameters>();
}

template<>
constexpr size_t payload_type<place_order>()
{
    return 9;
}

template<>
constexpr size_t buffer_size<pull_order>()
{
    return sizeof(exchange_id_t);
}

template<>
constexpr size_t payload_type<pull_order>()
{
    return 10;
}

inline size_t pack( void* buffer, size_t offset, const session& source )
{
    offset = ::pack( buffer, offset, &source.session_id_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const header& source )
{
    offset = ::pack( buffer, offset, &source.size_ );
    offset = ::pack( buffer, offset, &source.type_ );
    offset = ::pack( buffer, offset, &source.sequence_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const login& source )
{
    offset = ::pack( buffer, offset, source.user_id_, 16 );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const resend& source )
{
    offset = ::pack( buffer, offset, &source.sequence_from_ );
    offset = ::pack( buffer, offset, &source.sequence_to_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_parameters& source )
{
    offset = ::pack( buffer, offset, source.symbol_, 8 );
    offset = ::pack( buffer, offset, &source.type_ );
    offset = ::pack( buffer, offset, &source.side_ );
    offset = ::pack( buffer, offset, &source.quantity_ );
    offset = ::pack( buffer, offset, &source.price_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_rejected& source )
{
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_placed& source )
{
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_pulled& source )
{
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.leaves_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_executed& source )
{
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.exec_price_ );
    offset = ::pack( buffer, offset, &source.exec_quantity_ );
    offset = ::pack( buffer, offset, &source.leaves_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const place_order& source )
{
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const pull_order& source )
{
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, session& target )
{
    offset = ::unpack( buffer, offset, &target.session_id_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, header& target )
{
    offset = ::unpack( buffer, offset, &target.size_ );
    offset = ::unpack( buffer, offset, &target.type_ );
    offset = ::unpack( buffer, offset, &target.sequence_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, login& target )
{
    offset = ::unpack( buffer, offset, target.user_id_, 16 );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, resend& target )
{
    offset = ::unpack( buffer, offset, &target.sequence_from_ );
    offset = ::unpack( buffer, offset, &target.sequence_to_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_parameters& target )
{
    offset = ::unpack( buffer, offset, target.symbol_, 8 );
    offset = ::unpack( buffer, offset, &target.type_ );
    offset = ::unpack( buffer, offset, &target.side_ );
    offset = ::unpack( buffer, offset, &target.quantity_ );
    offset = ::unpack( buffer, offset, &target.price_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_rejected& target )
{
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_placed& target )
{
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_pulled& target )
{
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.leaves_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_executed& target )
{
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.exec_price_ );
    offset = ::unpack( buffer, offset, &target.exec_quantity_ );
    offset = ::unpack( buffer, offset, &target.leaves_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, place_order& target )
{
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, pull_order& target )
{
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    return offset;
}

inline std::ostream& operator<<( std::ostream& out, const session& in )
{
    out << "session: ";
    out << "session_id=" << in.session_id_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const header& in )
{
    out << "header: ";
    out << "size=" << in.size_;
    out << ", " << "type=" << in.type_;
    out << ", " << "sequence=" << in.sequence_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const login& in )
{
    out << "login: ";
    out << "user_id=" << in.user_id_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const resend& in )
{
    out << "resend: ";
    out << "sequence_from=" << in.sequence_from_;
    out << ", " << "sequence_to=" << in.sequence_to_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_parameters& in )
{
    out << "order_parameters: ";
    out << "symbol=" << in.symbol_;
    out << ", " << "type=" << in.type_;
    out << ", " << "side=" << in.side_;
    out << ", " << "quantity=" << in.quantity_;
    out << ", " << "price=" << in.price_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_rejected& in )
{
    out << "order_rejected: ";
    out << "transaction_id=" << in.transaction_id_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_placed& in )
{
    out << "order_placed: ";
    out << "transaction_id=" << in.transaction_id_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_pulled& in )
{
    out << "order_pulled: ";
    out << "transaction_id=" << in.transaction_id_;
    out << ", " << "leaves=" << in.leaves_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_executed& in )
{
    out << "order_executed: ";
    out << "transaction_id=" << in.transaction_id_;
    out << ", " << "exec_price=" << in.exec_price_;
    out << ", " << "exec_quantity=" << in.exec_quantity_;
    out << ", " << "leaves=" << in.leaves_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const place_order& in )
{
    out << "place_order: ";
    out << "transaction_id=" << in.transaction_id_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const pull_order& in )
{
    out << "pull_order: ";
    out << "transaction_id=" << in.transaction_id_;
    return out;
};

