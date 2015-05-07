#include "io.hpp"

#include <thread>

io io;

void input_thread( event_publisher* inp )
{
    io.accept( 14002, inp );
}

void journal_thread( event_subscriber* inp )
{
    inp->dispatch( [&]( const packed_event& ev, size_t r ) {
        return false;
    } );
}

void replicate_thread( event_subscriber* inp )
{
    inp->dispatch( [&]( const packed_event& ev, size_t r ) {
        return false;
    } );
}

void business_thread( event_subscriber* inp, event_publisher* out )
{
    inp->dispatch( [&]( const packed_event& ev, size_t r )
    {
        header hdr;
        ev.unpack( hdr );

        if( hdr.type_ == place_order::id )
        {
            place_order po;
            ev.unpack( po );

            out->publish( 2, [&]( packed_event& ev, size_t n )
            {
                if( n == 0 )
                {
                    order_placed op;
                    op.parameters_ = po.parameters_;
                    op.user_id_ = po.user_id_;
                    op.transaction_id_ = po.transaction_id_;
                    ev.pack( op );
                }
                else
                {
                    order_executed oe;
                    oe.parameters_ = po.parameters_;
                    oe.user_id_ = po.user_id_;
                    oe.transaction_id_ = po.transaction_id_;
                    oe.exec_price_ = po.parameters_.price_;
                    oe.exec_quantity_ = po.parameters_.quantity_;
                    oe.leaves_ = 0;
                    ev.pack( oe );
                }
            } );
        }

        return false;
    } );
}

void output_thread( event_subscriber* out )
{
    out->dispatch( []( const packed_event& ev, size_t r ) {
        std::cout << "> " << ev << std::endl;
        return false;
    } );
}

int main()
{
    const size_t Q = 8;

    event_publisher input( Q );
    event_publisher output( Q );

    event_subscriber& jnl_sub = input.subscribe();
    event_subscriber& rep_sub = jnl_sub.subscribe();
    event_subscriber& biz_sub = rep_sub.subscribe();

    std::thread out_thr( output_thread, &output.subscribe() );
    std::thread jnl_thr( journal_thread, &jnl_sub );
    std::thread rep_thr( replicate_thread, &rep_sub );
    std::thread biz_thr( business_thread, &biz_sub, &output );
    std::thread inp_thr( input_thread, &input );

    biz_thr.join();
    rep_thr.join();
    jnl_thr.join();
    inp_thr.join();
    out_thr.join();
}
