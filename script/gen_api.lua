function gen_struct( names )
    local id=0
    for key,name in pairs( names ) do
        print( 'struct ' .. name )
        print( '{' )
        for key,field in pairs( _G[name] ) do
            if field['length'] == nil then
                print( '    ' .. field['type'] .. ' ' .. field['name'] .. '_;' )
            else
                print( '    ' .. field['type'] .. ' ' .. field['name'] .. '_[' .. field['length'] .. '];' )
            end
        end
        print( '};\n' )
        id=id+1
    end
end

function gen_traits( names )
    local j=0
    for key,name in pairs( names ) do
        local i=0
        local s=''
        for key,field in pairs( _G[name] ) do
            if i>0 then
                s = s .. ' + '
            end
            if _G[field['type']] == nil then
                if field['length'] == nil then
                    s = s .. 'sizeof(' .. field['type'] .. ')'
                else
                    s = s .. field['length'] .. '*sizeof(' .. field['type'] .. ')'
                end
            else
                s = s .. 'buffer_size<' .. field['type'] .. '>()'
            end
            i=i+1
        end
        print( 'template<>' )
        print( 'constexpr size_t buffer_size<' .. name .. '>()' )
        print( '{' )
        print( '    return ' .. s .. ';' )
        print( '}\n' )
        print( 'template<>' )
        print( 'constexpr size_t payload_type<' .. name .. '>()' )
        print( '{' )
        print( '    return ' .. j .. ';' )
        print( '}\n' )
        j=j+1
    end
end

function gen_pack( names )
    for key,name in pairs( names ) do
        print( 'inline size_t pack( void* buffer, size_t offset, const ' .. name .. '& source )' )
        print( '{' )
        for key,field in pairs( _G[name] ) do
            if field['length'] == nil then
                print( '    offset = ::pack( buffer, offset, &source.' .. field['name'] .. '_ );' )
            else
                if _G[field['type']] == nil then
                    print( '    offset = ::pack( buffer, offset, source.' .. field['name'] .. '_, ' .. field['length'] .. ' );' )
                else
                    for i=1,field['length'] do
                        print( '    offset = ::pack( buffer, offset, &source.' .. field['name'] .. '_ );' )
                    end
                end
            end
        end
        print( '    return offset;' )
        print( '}\n' )
    end
end

function gen_unpack( names )
    for key,name in pairs( names ) do
        print( 'inline size_t unpack( const void* buffer, size_t offset, ' .. name .. '& target )' )
        print( '{' )
        for key,field in pairs( _G[name] ) do
            if field['length'] == nil then
                print( '    offset = ::unpack( buffer, offset, &target.' .. field['name'] .. '_ );' )
            else
                if _G[field['type']] == nil then
                    print( '    offset = ::unpack( buffer, offset, target.' .. field['name'] .. '_, ' .. field['length'] .. ' );' )
                else
                    for i=1,field['length'] do
                        print( '    offset = ::unpack( buffer, offset, &target.' .. field['name'] .. '_ );' )
                    end
                end
            end
        end
        print( '    return offset;' )
        print( '}\n' )
    end
end

function gen_ostream( names )
    for key,name in pairs( names ) do
        print( 'inline std::ostream& operator<<( std::ostream& out, const ' .. name .. '& in )' )
        print( '{' )
        print( '    out << \"' .. name .. ': \";' )
        local i=0
        for key,field in pairs( _G[name] ) do
            s = '    out'
            if i>0 then
                s = s .. ' << \", \"'
            end
            s = s .. ' << \"' .. field['name'] .. '=\" << in.' .. field['name'] .. '_;'
            i=i+1
            print( s )
        end
        print( '    return out;' )
        print( '};\n' )
    end
end
