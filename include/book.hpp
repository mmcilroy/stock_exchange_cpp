#pragma once

#include "structs.hpp"

struct order_state
{
    order_parameters parameters_;
    transaction_id transaction_;
    quantity leaves_;
};

struct book
{
    typedef std::multimap< price, order, std::greater< price > > buy_orders;
    typedef std::multimap< price, order, std::less< price > > sell_orders;

    void operator( const place_order& );
};
