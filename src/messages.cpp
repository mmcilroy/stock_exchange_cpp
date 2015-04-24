#include "messages.hpp"

std::ostream& operator<<( std::ostream& o, header v )
{
    o << "length=[" << v.length() << "] ";
    o << "type=[" << v.type() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, login v )
{
    o << "user_id=[" << v.user_id() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, order_parameters v )
{
    o << "type=[" << v.type() << "] ";
    o << "side=[" << v.side() << "] ";
    o << "quantity=[" << v.quantity() << "] ";
    o << "price=[" << v.price() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, place_order v )
{
    o << "user_id=[" << v.user_id() << "] ";
    o << "transaction_id=[" << v.transaction_id() << "] ";
    o << "parameters=[" << v.parameters() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, pull_order v )
{
    o << "user_id=[" << v.user_id() << "] ";
    o << "transaction_id=[" << v.transaction_id() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, order_rejected v )
{
    o << "user_id=[" << v.user_id() << "] ";
    o << "transaction_id=[" << v.transaction_id() << "] ";
    o << "parameters=[" << v.parameters() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, order_placed v )
{
    o << "user_id=[" << v.user_id() << "] ";
    o << "transaction_id=[" << v.transaction_id() << "] ";
    o << "parameters=[" << v.parameters() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, order_pulled v )
{
    o << "user_id=[" << v.user_id() << "] ";
    o << "transaction_id=[" << v.transaction_id() << "] ";
    o << "leaves_quantity=[" << v.leaves_quantity() << "] ";
    o << "parameters=[" << v.parameters() << "] ";
    return o;
}

std::ostream& operator<<( std::ostream& o, order_executed v )
{
    o << "user_id=[" << v.user_id() << "] ";
    o << "transaction_id=[" << v.transaction_id() << "] ";
    o << "exec_price=[" << v.exec_price() << "] ";
    o << "exec_quantity=[" << v.exec_quantity() << "] ";
    o << "leaves_quantity=[" << v.leaves_quantity() << "] ";
    o << "parameters=[" << v.parameters() << "] ";
    return o;
}
