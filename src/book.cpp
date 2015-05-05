#include <cassert>
#include <map>
#include <iostream>

typedef uint32_t price;
typedef uint32_t quantity;

enum class side { buy, sell };

struct order
{
    order( uint32_t oq ) :
        order_qty_( oq ),
        leaves_qty_( oq )
    {
    }

    order( uint32_t oq, uint32_t lq ) :
        order_qty_( oq ),
        leaves_qty_( lq )
    {
        assert( lq <= oq );
    }

    uint32_t order_qty_;
    uint32_t leaves_qty_;
};

class book
{
public:
    book()
    {
        buys_.emplace( 1, order( 2 ) );
        buys_.emplace( 2, order( 2 ) );
        buys_.emplace( 3, order( 2 ) );
    }

    template< typename F >
    void execute( side in_side, price in_price, quantity in_quantity, F& callback )
    {
        quantity leaves_qty = in_quantity;

        auto i = buys_.begin();
        while( i != buys_.end() )
        {
            price book_price = i->first;
            order& book_order = i->second;

            if( leaves_qty > 0 && book_price >= in_price )
            {
                quantity exec_qty = std::min( leaves_qty, book_order.leaves_qty_ );
                book_order.leaves_qty_ -= exec_qty;
                leaves_qty -= exec_qty;
                callback( book_order, book_price, exec_qty );

                if( book_order.leaves_qty_ <= 0 ) {
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
            sells_.emplace( in_price, order( in_quantity, leaves_qty ) );
        }
    }

    void print()
    {
        for( auto i = buys_.begin(); i != buys_.end(); i++ ) {
            std::cout << "B " << i->second.leaves_qty_ << " @ " << i->first << std::endl;
        }

        for( auto j = sells_.begin(); j != sells_.end(); j++ ) {
            std::cout << "S " << j->second.leaves_qty_ << " @ " << j->first << std::endl;
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

    book b;
    b.print();
    b.execute( side::sell, 3, 8, f );
    b.print();
}
