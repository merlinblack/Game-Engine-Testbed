doubleTab = doubleTab or {}

function autoComplete( str )
    local prefixend = string.find( str:reverse(), '[() %[%]=+/,%%]' )
    local prefix = ''
    local posibles

    if prefixend then
        prefix = string.sub( str, 1, #str - prefixend + 1 )
        str = string.sub( str, #str - prefixend + 2 )
    end

    str, posibles = complete(str)

    if #posibles > 1 then
        if doubleTab.str == str then
            print( table.unpack( posibles ) )
        else
            doubleTab.str = str
        end
    end

    return prefix..str
end

function autoCompleteClear()
    doubleTab.str = nil
end

function complete( str )
    local possibles = getCompletions( str )
    if #possibles > 0 then
        str = string.sub( possibles[1], 1, getIdenticalPrefixLength( possibles, #str ) )
    end
    return str, possibles
end

function getCompletions( str )
    local g = _G
    local ret = {}
    local dotpos = string.find( str:reverse(), '[%.:]' )
    local prefix = ''
    local dottype = ''

    if dotpos ~= nil then
        dotpos = #str - dotpos
        prefix = string.sub( str, 1, dotpos )
        dottype = string.sub( str, dotpos + 1, dotpos + 1 )
        g = getTable(prefix)
        str = string.sub( str, dotpos + 2 )
    end

    if g == nil then
        return {}
    end

    -- Retrieve class info if any
    for k,v in pairs( getClassInfo( g ) ) do
        if string.find( v, str ) == 1 then
            table.insert( ret, prefix .. dottype .. v )
        end
    end

    if type( g ) == 'table' then
        for k,v in pairs(g) do
            if string.find( k, str ) == 1 and string.sub(k,1,1) ~= '_' then
                table.insert( ret, prefix .. dottype .. k )
            end
        end
    end

    return ret
end

function getTable( tblname )
    --print( 'Looking up:', tblname )
    local lastdot = string.find( tblname:reverse(), '%.' )
    if lastdot == nil then
        return _G[tblname]
    end
    local prefix = string.sub( tblname, 1, #tblname - lastdot )
    local tbl = getTable( prefix )
    if type(tbl) ~= 'table' then
        if type(tbl) ~= 'userdata' then
            error( prefix .. ' is not a table or class.' )
        end
        tbl = infotable( tbl )
    end
    local subscript = string.sub( tblname, #tblname - string.find( tblname:reverse(), '%.' ) + 2 )
    --print( "Subscript:", subscript, tblname )
    return tbl[subscript]
end

function getIdenticalPrefixLength( tbl, start )
    if #tbl == 0 then return start end
    local l = start
    local str
    local allSame = true
    while allSame == true do
        if l > #tbl[1] then return #tbl[1] end
        str = string.sub( tbl[1], 1, l )
        table.foreach( tbl,
            function(k,v)
                if string.find( v, str ) ~= 1 then
                    allSame = false
                end
            end
        )
        l = l + 1
    end
    return l - 2
end
