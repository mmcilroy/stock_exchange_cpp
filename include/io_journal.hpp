#pragma once

#include "io_event.hpp"
#include "configuration.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

class io_journal
{
public:
    io_journal( const std::string& title, bool write_index=false );

    template< typename F >
    void read( session_id_t, sequence_t, sequence_t, F );

    void write( const io_event& );

private:
    std::fstream& get_index( session_id_t );

    std::string title_;
    std::fstream event_file_;
    std::vector< std::unique_ptr< std::fstream > > index_files_;
    bool write_index_;
};

struct index_record
{
    sequence_t sequence_;
    uint32_t offset_;
    uint16_t size_;
};

std::ostream& operator<<( std::ostream& o, const index_record& ir )
{
    o << "index_record - "
      << "sequence=" << ir.sequence_ << ", "
      << "offset=" << ir.offset_ << ", "
      << "size=" << ir.size_;
    return o;
}

inline io_journal::io_journal( const std::string& title, bool write_index ) :
    title_( title ),
    index_files_( max_sessions ),
    write_index_( write_index )
{
    std::string path = title + ".bin";
    std::ofstream( path.c_str(), std::ofstream::binary | std::ofstream::app );
    event_file_.open( path.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary );

    if( !event_file_.is_open() ) {
        // error
    }
}

template< typename F >
inline void io_journal::read( session_id_t sess, sequence_t from, sequence_t to, F handler )
{
    auto& index = get_index( sess );

    index_record ir;
    index.seekg( from * sizeof( index_record ), std::ios::beg );
    index.read( (char*)&ir, sizeof( index_record ) );
    std::cout << ir << std::endl;

    io_event e;

    event_file_.seekg( ir.offset_ );
    event_file_.read( (char*)e.session_buffer(), ir.size_ );

    handler( e );
}

inline void io_journal::write( const io_event& ev )
{
    if( write_index_ )
    {
        header h;
        session s;
        ev.unpack( h );
        ev.unpack( s );

        auto& index = get_index( s.session_id_ );

        index_record ir;
        ir.sequence_ = h.sequence_;
        ir.offset_ = event_file_.tellp();
        ir.size_ = ev.size();

        std::cout << ir << std::endl;

        index.seekp( ir.sequence_ * sizeof( index_record ), std::ios_base::beg );
        index.write( (const char*)&ir, sizeof( index_record ) );
        index.flush();
    }

    event_file_.seekp( 0, std::ios_base::end );
    event_file_.write( (const char*)ev.session_buffer(), ev.size() );
    event_file_.flush();
}

inline std::fstream& io_journal::get_index( session_id_t sess )
{
    auto& index = index_files_[ sess ];
    if( !index )
    {
        std::stringstream path;
        path << title_ << "_index_" << sess << ".bin";

        std::ofstream( path.str().c_str(), std::ofstream::binary | std::ofstream::app );
        index = std::move( std::unique_ptr< std::fstream >( new std::fstream ) );
        index->open( path.str().c_str(), std::fstream::in | std::fstream::out | std::fstream::binary );
    }

    return *index;
}
