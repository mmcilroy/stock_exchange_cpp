#include "events.hpp"
#include <cassert>
#include <cstring>

template< typename T, typename U >
void test( T& a )
{
    U p;
    pack( p.buf_, 0, a );

    T b;
    unpack( p.buf_, 0, b );

    assert( memcmp( &a, &b, sizeof( a ) ) == 0 );
    std::cout << b << std::endl;
}

void fill_order_parameters( order_parameters& op )
{
    strcpy( op.symbol_, "test" );
    op.type_ = 1;
    op.side_ = 2;
    op.quantity_ = 3;
    op.price_ = 4;
}

int main()
{
    {
        header h;
        h.type_ = 1;
        h.length_ = 2;
        test< header, packed_header >( h );
    }

    {
        login l;
        strcpy( l.user_id_, "test" );
        test< login, packed_login >( l );
    }

    {
        order_parameters op;
        fill_order_parameters( op );
        test< order_parameters, packed_order_parameters >( op );
    }

    {
        place_order po;
        po.user_id_ = 1;
        po.transaction_id_ = 2;
        fill_order_parameters( po.parameters_ );
        test< place_order, packed_place_order >( po );
    }

    {
        pull_order po;
        po.user_id_ = 1;
        po.transaction_id_ = 2;
        test< pull_order, packed_pull_order >( po );
    }

    {
        order_rejected r;
        r.user_id_ = 1;
        r.transaction_id_ = 2;
        fill_order_parameters( r.parameters_ );
        test< order_rejected, packed_order_rejected >( r );
    }

    {
        order_placed p;
        p.user_id_ = 1;
        p.transaction_id_ = 2;
        fill_order_parameters( p.parameters_ );
        test< order_placed, packed_order_placed >( p );
    }

/*
order_pulled = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='leaves_quantity', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

order_executed = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='exec_price', type='uint32_t' },
    { name='exec_quantity', type='uint32_t' },
    { name='leaves_quantity', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}
*/
}
