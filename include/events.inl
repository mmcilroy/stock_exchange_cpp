struct header
{
    static constexpr uint16_t id = 0;

    uint16_t length_;
    uint16_t type_;
};

struct login
{
    static constexpr uint16_t id = 1;

    char user_id_[16];
};

struct order_parameters
{
    static constexpr uint16_t id = 2;

    char symbol_[8];
    uint16_t type_;
    uint16_t side_;
    uint32_t quantity_;
    uint32_t price_;
};

struct order_rejected
{
    static constexpr uint16_t id = 3;

    uint32_t user_id_;
    uint32_t transaction_id_;
    order_parameters parameters_;
};

struct order_placed
{
    static constexpr uint16_t id = 4;

    uint32_t user_id_;
    uint32_t transaction_id_;
    order_parameters parameters_;
};

struct order_pulled
{
    static constexpr uint16_t id = 5;

    uint32_t user_id_;
    uint32_t transaction_id_;
    uint32_t leaves_quantity_;
    order_parameters parameters_;
};

struct order_executed
{
    static constexpr uint16_t id = 6;

    uint32_t user_id_;
    uint32_t transaction_id_;
    uint32_t exec_price_;
    uint32_t exec_quantity_;
    uint32_t leaves_quantity_;
    order_parameters parameters_;
};

struct place_order
{
    static constexpr uint16_t id = 7;

    uint32_t user_id_;
    uint32_t transaction_id_;
    order_parameters parameters_;
};

struct pull_order
{
    static constexpr uint16_t id = 8;

    uint32_t user_id_;
    uint32_t transaction_id_;
};

typedef packed_buffer< header, sizeof(uint16_t) + sizeof(uint16_t) > packed_header;
typedef packed_buffer< login, 16*sizeof(char) > packed_login;
typedef packed_buffer< order_parameters, 8*sizeof(char) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t) > packed_order_parameters;
typedef packed_buffer< order_rejected, sizeof(uint32_t) + sizeof(uint32_t) + packed_order_parameters::size > packed_order_rejected;
typedef packed_buffer< order_placed, sizeof(uint32_t) + sizeof(uint32_t) + packed_order_parameters::size > packed_order_placed;
typedef packed_buffer< order_pulled, sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + packed_order_parameters::size > packed_order_pulled;
typedef packed_buffer< order_executed, sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + packed_order_parameters::size > packed_order_executed;
typedef packed_buffer< place_order, sizeof(uint32_t) + sizeof(uint32_t) + packed_order_parameters::size > packed_place_order;
typedef packed_buffer< pull_order, sizeof(uint32_t) + sizeof(uint32_t) > packed_pull_order;

inline size_t pack( void* buffer, size_t offset, const header& source )
{
    offset = ::pack( buffer, offset, &source.length_ );
    offset = ::pack( buffer, offset, &source.type_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const login& source )
{
    offset = ::pack( buffer, offset, source.user_id_, 16 );
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
    offset = ::pack( buffer, offset, &source.user_id_ );
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_placed& source )
{
    offset = ::pack( buffer, offset, &source.user_id_ );
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_pulled& source )
{
    offset = ::pack( buffer, offset, &source.user_id_ );
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.leaves_quantity_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const order_executed& source )
{
    offset = ::pack( buffer, offset, &source.user_id_ );
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.exec_price_ );
    offset = ::pack( buffer, offset, &source.exec_quantity_ );
    offset = ::pack( buffer, offset, &source.leaves_quantity_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const place_order& source )
{
    offset = ::pack( buffer, offset, &source.user_id_ );
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    offset = ::pack( buffer, offset, &source.parameters_ );
    return offset;
}

inline size_t pack( void* buffer, size_t offset, const pull_order& source )
{
    offset = ::pack( buffer, offset, &source.user_id_ );
    offset = ::pack( buffer, offset, &source.transaction_id_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, header& target )
{
    offset = ::unpack( buffer, offset, &target.length_ );
    offset = ::unpack( buffer, offset, &target.type_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, login& target )
{
    offset = ::unpack( buffer, offset, target.user_id_, 16 );
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
    offset = ::unpack( buffer, offset, &target.user_id_ );
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_placed& target )
{
    offset = ::unpack( buffer, offset, &target.user_id_ );
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_pulled& target )
{
    offset = ::unpack( buffer, offset, &target.user_id_ );
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.leaves_quantity_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, order_executed& target )
{
    offset = ::unpack( buffer, offset, &target.user_id_ );
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.exec_price_ );
    offset = ::unpack( buffer, offset, &target.exec_quantity_ );
    offset = ::unpack( buffer, offset, &target.leaves_quantity_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, place_order& target )
{
    offset = ::unpack( buffer, offset, &target.user_id_ );
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    offset = ::unpack( buffer, offset, &target.parameters_ );
    return offset;
}

inline size_t unpack( const void* buffer, size_t offset, pull_order& target )
{
    offset = ::unpack( buffer, offset, &target.user_id_ );
    offset = ::unpack( buffer, offset, &target.transaction_id_ );
    return offset;
}

inline std::ostream& operator<<( std::ostream& out, const header& in )
{
    out << "header: ";
    out << "length=" << in.length_;
    out << ", " << "type=" << in.type_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const login& in )
{
    out << "login: ";
    out << "user_id=" << in.user_id_;
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
    out << "user_id=" << in.user_id_;
    out << ", " << "transaction_id=" << in.transaction_id_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_placed& in )
{
    out << "order_placed: ";
    out << "user_id=" << in.user_id_;
    out << ", " << "transaction_id=" << in.transaction_id_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_pulled& in )
{
    out << "order_pulled: ";
    out << "user_id=" << in.user_id_;
    out << ", " << "transaction_id=" << in.transaction_id_;
    out << ", " << "leaves_quantity=" << in.leaves_quantity_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const order_executed& in )
{
    out << "order_executed: ";
    out << "user_id=" << in.user_id_;
    out << ", " << "transaction_id=" << in.transaction_id_;
    out << ", " << "exec_price=" << in.exec_price_;
    out << ", " << "exec_quantity=" << in.exec_quantity_;
    out << ", " << "leaves_quantity=" << in.leaves_quantity_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const place_order& in )
{
    out << "place_order: ";
    out << "user_id=" << in.user_id_;
    out << ", " << "transaction_id=" << in.transaction_id_;
    out << ", " << "parameters=" << in.parameters_;
    return out;
};

inline std::ostream& operator<<( std::ostream& out, const pull_order& in )
{
    out << "pull_order: ";
    out << "user_id=" << in.user_id_;
    out << ", " << "transaction_id=" << in.transaction_id_;
    return out;
};

