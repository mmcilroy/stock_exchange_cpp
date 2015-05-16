#include "io_controller.hpp"
#include "publisher.hpp"

typedef publisher< io_event, blocking_sequence > io_event_publisher;
typedef subscriber< io_event, blocking_sequence > io_event_subscriber;



// ----------------------------------------------------------------------------
void jnl_thr_fn( io_event_subscriber* sub )
{
    io_journal ioj( "in", false );
    sub->dispatch( [&]( const io_event& ev, size_t rem )
    {
        ioj.write( ev );
        return false;
    } );
}



// ----------------------------------------------------------------------------
void biz_thr_fn( io_event_subscriber* sub, io_event_publisher* pub )
{
    sub->dispatch( [&]( const io_event& ei, size_t rem )
    {
        header mh;
        ei.unpack( mh );

        session sh;
        ei.unpack( sh );

        if( mh.type_ == payload_type< place_order >() )
        {
            place_order po;
            ei.unpack( po );

            pub->publish( 1+po.parameters_.quantity_, [&]( io_event& eo, size_t n )
            {
                if( n == 0 )
                {
                    order_placed op;
                    op.parameters_ = po.parameters_;
                    op.transaction_id_ = po.transaction_id_;
                    eo.pack( op );
                    eo.pack( sh );
                }
                else
                {
                    order_executed oe;
                    oe.parameters_ = po.parameters_;
                    oe.transaction_id_ = po.transaction_id_;
                    oe.exec_price_ = po.parameters_.price_;
                    oe.exec_quantity_ = 1;
                    oe.leaves_ = po.parameters_.quantity_-n;
                    eo.pack( oe );
                    eo.pack( sh );
                }
            } );
        }

        return false;
    } );
}



// ----------------------------------------------------------------------------
void out_thr_fn( io_controller* ioc, io_event_subscriber* sub )
{
    sub->dispatch( [&]( const io_event& ev, size_t rem )
    {
        ioc->write( ev );
        return false;
    } );
}



// ----------------------------------------------------------------------------
int main()
{
    const size_t Q = 8;

    io_event_publisher inp_pub( Q );
    io_event_publisher out_pub( Q );
    io_controller ioc;

    io_event_subscriber& jnl_sub = inp_pub.subscribe();
    io_event_subscriber& biz_sub = jnl_sub.subscribe();

    std::thread out_thr( out_thr_fn, &ioc, &out_pub.subscribe() );
    std::thread biz_thr( biz_thr_fn, &biz_sub, &out_pub );
    std::thread jnl_thr( jnl_thr_fn, &jnl_sub );

    ioc.accept( 14002, [ &inp_pub ]( const io_event& ea ) {
        std::cout << "< " << ea << std::endl;
        inp_pub.publish( 1, [ &ea ]( io_event& ep, size_t n ) {
            ep = ea;
        } );
    } );

    jnl_thr.join();
    biz_thr.join();
    out_thr.join();
}
