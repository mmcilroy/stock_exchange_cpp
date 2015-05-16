#pragma once

#include "io_event.hpp"
#include "configuration.hpp"
#include <fstream>
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
    std::string title_;
    std::fstream events_;
    std::vector< std::unique_ptr< std::fstream > > indexes_;
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
    indexes_( max_sessions ),
    write_index_( write_index )
{
    std::string path = title + ".bin";
    std::ofstream( path.c_str(), std::ofstream::binary | std::ofstream::app );
    events_.open( path.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary );

    if( !events_.is_open() ) {
        // error
    }
}

template< typename F >
inline void io_journal::read( session_id_t sess, sequence_t from, sequence_t to, F handler )
{
}

inline void io_journal::write( const io_event& ev )
{
    events_.seekp( 0, std::ios_base::end );
    events_.write( (const char*)ev.session_buffer(), ev.size() );
    events_.flush();

    if( write_index_ )
    {
        session s;
        ev.unpack( s );

        std::unique_ptr< std::fstream >& index = indexes_[ s.session_id_ ];
        index->seekp( 0, std::ios_base::end );
        //index->write( ... );
        index->flush();
    }
}
