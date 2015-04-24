#pragma once

#include "structs.hpp"

struct order
{
    order_parameters params_;
    quantity exec_qty_;
};

struct book
{
    typedef std::multimap< price, order, std::greater< price > > buy_orders;
    typedef std::multimap< price, order, std::less< price > > sell_orders;

    void execute( const order_parameters& )
    {
    }

    quantity executable_quantity( const order_parameters& )
    {
    }
};
