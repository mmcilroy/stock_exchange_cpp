#include "events.hpp"
#include <map>

struct order_state
{
    order_parameters parameters_;
    exchange_id_t transaction_;
    order_quantity_t leaves_;
};

class book
{
public:
    template< typename F >
    void operator( const place_order& place, F& callback )
    {
        order_quantity_t leaves_qty = place.parameters_.quantity_;

        auto i = buys_.begin();
        while( i != buys_.end() )
        {
            order_price_t price = i->first;
            order_state& state = i->second;

            if( leaves_qty > 0 && price >= place.parameters_.price_ )
            {
                order_quantity_t exec_qty = std::min( leaves_qty, state.leaves_ );
                state.leaves_ -= exec_qty;
                leaves_qty -= exec_qty;

                callback( book_order, book_price, exec_qty );

                if( state.leaves_ <= 0 ) {
                    buys_.erase( i++ );
                } else {
                    ++i;
                }
            }
            else
            {
                break;
            }
        }

        if( leaves_qty > 0 ) {
            //sells_.emplace( in_price, order( in_quantity, leaves_qty ) );
        }
    }

private:
    typedef std::multimap< price, order, std::greater< price > > buys;
    typedef std::multimap< price, order, std::less< price > > sells;

    buys buys_;
    sells sells_;
};

int main()
{
    auto f = []( const order& o, price p, quantity q ){
        std::cout << "execute " << q << " @ " << p << std::endl;
    };

    place_order po;
    po.user_id_ = 1;
    po.transaction_id_ = 2;
    po.parameters_.type_ = order_type_t::limit;
    po.parameters_.side_ = order_side_t::buy;
    po.parameters_.quantity_ = 8;
    po.parameters_.price_ = 9;
    strcpy( po.parameters_.symbol_, "test" );

    book b;
    b( po, f );
}
