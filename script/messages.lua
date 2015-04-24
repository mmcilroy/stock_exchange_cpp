primitives = {
    int8_t      = 1,
    int16_t     = 2,
    int32_t     = 4,
    int64_t     = 8,
    uint8_t     = 1,
    uint16_t    = 2,
    uint32_t    = 4,
    uint64_t    = 8
}

header = {
    { name='length', type='uint16_t' },
    { name='type', type='uint16_t' }
}

login = {
    { name='user_id', type='uint32_t' }
}

order_parameters = {
    { name='type', type='uint16_t' },
    { name='side', type='uint16_t' },
    { name='quantity', type='uint32_t' },
    { name='price', type='uint32_t' }
}

place_order = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

pull_order = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' }
}

order_rejected = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

order_placed = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

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

names = {
    'header',
    'login',
    'order_parameters',
    'place_order',
    'pull_order',
    'order_rejected',
    'order_placed',
    'order_pulled',
    'order_executed'
}

function message_length( message )
    local len = 0
    for key,field in pairs( message ) do
        len = len + field_length( field )
    end
    return len
end

function field_length( field )
    len = field['length']
    if len == nil then
        len = 1
    end
    if field['type'] == 'string_t' then
        return len
    elseif primitives[field['type']] ~= nil then
        return len * primitives[field['type']]
    else
        return len * message_length( _G[field['type']] )
    end
end

function gen_primitive_field( field, offset )
    out = out .. '    void ' .. field['name'].. '( ' .. field['type'].. ' v ) {\n'
    out = out .. '        *(' .. field['type'].. '*)(buf_+' .. offset .. ') = v;\n'
    out = out .. '    }\n\n'
    out = out .. '    ' .. field['type'].. ' ' .. field['name'].. '() const {\n'
    out = out .. '        return *(' .. field['type'].. '*)(buf_+' .. offset .. ');\n'
    out = out .. '    }\n\n'
end

function gen_primitive_array_field( field, offset )
    out = out .. '    void ' .. field['name'].. '( ' .. field['type'].. '* v, size_t n ) {\n'
    out = out .. '        memcpy( buf_+' .. offset .. ', v, n );\n'
    out = out .. '    }\n\n'
    out = out .. '    ' .. field['type'] .. ' ' .. field['name'].. '( size_t n ) const {\n'
    out = out .. '        return *(' .. field['type'].. '*)(buf_+' .. offset .. '+n*sizeof(' .. field['type'] .. '));\n'
    out = out .. '    }\n\n'
    out = out .. '    static constexpr size_t ' .. field['name'].. '_length() {\n'
    out = out .. '        return ' .. field['length'].. ';\n'
    out = out .. '    }\n\n'
end

function gen_message_field( field, offset )
    out = out .. '    ' .. field['type'].. ' ' .. field['name'].. '() {\n'
    out = out .. '        return ' .. field['type'].. '(buf_+' .. offset .. ');\n'
    out = out .. '    }\n\n'
end

function gen_message_array_field( field, offset )
    out = out .. '    ' .. field['type'].. ' ' .. field['name'].. '( size_t n ) {\n'
    out = out .. '        return ' .. field['type'].. '(buf_+' .. offset .. '+n*' .. message_length( _G[field['type']] ) .. ');\n'
    out = out .. '    }\n\n'
end

function gen_string_field( field, offset )
    out = out .. '    void ' .. field['name'].. '( const char* v ) {\n'
    out = out .. '        strcpy( buf_+' .. offset .. ', v );\n'
    out = out .. '    }\n\n'
    out = out .. '    const char* ' .. field['name'].. '() const {\n'
    out = out .. '        return buf_+' .. offset .. ';\n'
    out = out .. '    }\n\n'
    out = out .. '    static constexpr size_t ' .. field['name'].. '_length() {\n'
    out = out .. '        return ' .. field['length'].. ';\n'
    out = out .. '    }\n\n'
end

function gen_field( field, offset )
    if field['type'] == 'string_t' then
        gen_string_field( field, offset )
    elseif primitives[field['type']] ~= nil then
        if field['length'] == nil then
            gen_primitive_field( field, offset )
        else
            gen_primitive_array_field( field, offset )
        end
    else
        if field['length'] == nil then
            gen_message_field( field, offset )
        else
            gen_message_array_field( field, offset )
        end
    end
    return offset + field_length( field )
end

function gen_class( name )
    local offset = 0
    out = out .. 'class ' .. name .. '\n{\n'
    out = out .. 'private:\n'
    out = out .. '    char* buf_;\n\n'
    out = out .. 'public:\n'
    out = out .. '    ' .. name .. '( void* buf ) : buf_( (char*)buf ) {\n'
    out = out .. '        ;\n'
    out = out .. '    }\n\n'
    out = out .. '    static constexpr size_t size() {\n'
    out = out .. '        return ' .. message_length( _G[name] ) .. ';\n'
    out = out .. '    }\n\n'
    out = out .. '    static constexpr size_t id() {\n'
    out = out .. '        return ' .. message_id .. ';\n'
    out = out .. '    }\n\n'
    out = out .. '    const char* buffer() {\n'
    out = out .. '        return buf_;\n'
    out = out .. '    }\n\n'
    for key,field in pairs( _G[name] ) do
        offset = gen_field( field, offset )
    end
    out = out .. '};\n\n'
    message_id = message_id + 1
end

function gen_ostream( name )
    out = out .. 'std::ostream& operator<<( std::ostream& o, ' .. name .. ' v )\n{\n'
    for key,field in pairs( _G[name] ) do
        if field['type'] == 'string_t' or field['length'] == nil then
            out = out .. '    o << \"' .. field['name'] .. '=[\" << v.' .. field['name'] .. '() << \"] \";\n'
        else
            out = out .. '    o << \"' .. field['name'] .. '=[\";\n'
            out = out .. '    for( size_t i=0; i<' .. field['length'] .. '; i++ ) {\n'
            out = out .. '        o << v.' .. field['name'] .. '( i ) << \" \";\n'
            out = out .. '    }\n'
            out = out .. '    o << \"] \";\n'
        end
    end
    out = out .. '    return o;\n'
    out = out .. '}\n\n'
end

function gen( names )
    for k,v in pairs( names ) do
        gen_class( v )
    end
    for k,v in pairs( names ) do
        gen_ostream( v )
    end
end

message_id = 0

out = ''
out = out .. '#pragma once\n\n'
out = out .. '#include <cstdint>\n'
out = out .. '#include <cstdlib>\n'
out = out .. '#include <cstring>\n'
out = out .. '#include <iostream>\n\n'

gen( names )

print( out )
