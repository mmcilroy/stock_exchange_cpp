#include "io.hpp"

#include <thread>

io io;

void input_thread( event_publisher* inp )
{
    io.accept( 14002, inp );
}

void journal_thread( event_subscriber* inp )
{
    inp->dispatch( [&]( const event& e, size_t r ) {
        return false;
    } );
}

void replicate_thread( event_subscriber* inp )
{
    inp->dispatch( [&]( const event& e, size_t r ) {
        return false;
    } );
}

void business_thread( event_subscriber* inp, event_publisher* out )
{
    inp->dispatch( [&]( const event& e, size_t r ) {
        header h;
        unpack( e.header_.buf_, 0, h );

        if( h.type_ == place_order::id )
        {
            place_order po;
            unpack( e.payload_.buf_, 0, po );
            std::cout << po << std::endl;
        }

        return false;
    } );
}

void output_thread( event_subscriber* out )
{
    out->dispatch( []( const event& e, size_t r ) {
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
