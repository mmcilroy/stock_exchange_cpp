function gen_struct( names )
    for key,name in pairs( names ) do
        print( 'struct ' .. name )
        print( '{' )
        for key,field in pairs( _G[name] ) do
            print( '    ' .. field['type'] .. ' ' .. field['name'] .. '_;' )
        end    
        print( '};\n' )
    end
end

function gen_packed_struct( names )
    print( 'template< typename T, size_t N >' )
    print( 'struct packed_buffer' )
    print( '{' )
    print( '    static constexpr size_t size = N;\n' )
    print( '    void pack( const T& source )' )
    print( '    {' )
    print( '        pack( source, buf_, 0, size );' )
    print( '    }\n' )
    print( '    void unpack( T& target ) const' )
    print( '    {' )
    print( '        unpack( target, buf_, 0, size );' )
    print( '    }\n' )
    print( '    char buf_[ size ];' )
    print( '};\n' )

    for key,name in pairs( names ) do
        local i=0
        local s=''
        for key,field in pairs( _G[name] ) do
            if i>0 then
                s = s .. ' + '
            end
            if _G[field['type']] == nil then
                s = s .. 'sizeof(' .. field['type'] .. ')'
            else
                s = s .. 'packed_' .. field['type'] .. '::size'
            end
            i=i+1
        end
        print( 'typedef packed_buffer< ' .. name .. ', ' .. s .. ' > packed_' .. name .. ';' )
    end

    print( '' )
end

function gen_proto( names )
    local types = {}
    for key,name in pairs( names ) do
        for key,field in pairs( _G[name] ) do
          types[field['type']] = true
        end
    end
    for key,type in pairs( types ) do
        if _G[key] == nil then
            print( 'size_t pack( const ' .. key .. '& source, void* buffer, size_t offset, size_t length )' )
            print( '{' )
            print( '    *(' .. key .. '*)((char*)buffer+offset) = source;' )
            print( '    return offset + sizeof(source);' )
            print( '}\n' )

            print( 'size_t unpack( ' .. key .. '& target, const void* buffer, size_t offset, size_t length )' )
            print( '{' )
            print( '    target = *(' .. key .. '*)((char*)buffer+offset);' )
            print( '    return offset + sizeof(target);' )
            print( '}\n' )
        end
    end
end

function gen_pack( names )
    for key,name in pairs( names ) do
        print( 'size_t pack( const ' .. name .. '& source, void* buffer, size_t offset, size_t length )' )
        print( '{' )
        for key,field in pairs( _G[name] ) do
            print( '    offset = pack( source.' .. field['name'] .. '_, buffer, offset, length );' )
        end
        print( '    return offset;' )
        print( '}\n' )
    end
end

function gen_unpack( names )
    for key,name in pairs( names ) do
        print( 'size_t unpack( ' .. name .. '& target, const void* buffer, size_t offset, size_t length )' )
        print( '{' )
        for key,field in pairs( _G[name] ) do
            print( '    offset = unpack( target.' .. field['name'] .. '_, buffer, offset, length );' )
        end
        print( '    return offset;' )
        print( '}\n' )
    end
end

function gen_union( names )
    print( 'union u' )
    print( '{' )
    for key,name in pairs( names ) do
        print( '    packed_' .. name .. ' ' .. name .. '_;' )
    end
    print( '};' )
end
