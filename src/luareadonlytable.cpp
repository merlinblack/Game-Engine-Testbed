#include <luareadonlytable.h>

using namespace luabridge;

int readonlyTableError( lua_State* L )
{
    return luaL_error( L, "Attempt to modify a readonly table." );
}

int readonlyTableNext( lua_State* L )
{
    lua_getmetatable( L, 1 ); // Get read only table's metatable
    lua_getfield( L, 3, "__index" ); // Get actual table
    lua_pushvalue( L, 2 );
    if( lua_next( L, 4 ) )
    {
        lua_remove( L, 3 ); // Meta table
        lua_remove( L, 3 ); // Real table
        lua_remove( L, 2 ); // Input index
        lua_remove( L, 1 ); // Input table
        return 2;
    }
    else
    {
        return 0;
    }
}

int getReadonlyTableNext( lua_State* L )
{
    lua_pushcfunction( L, readonlyTableNext );
    lua_insert( L, 1 );
    lua_pushnil( L );

    return 3;
}

LuaRef makeReadonlyProxy( lua_State* L, LuaRef table )
{
    if( table.isTable() == false )
        throw std::invalid_argument( "makeTableReadOnly not given table" );

    LuaRef metatable = newTable( L );
    metatable["__index"] = table;
    metatable["__newindex"] = readonlyTableError;
    metatable["__metatable"] = false;
    metatable["__pairs"] = getReadonlyTableNext;

    LuaRef proxy = newTable( L );
    proxy.push( L );
    metatable.push( L );
    lua_setmetatable( L, -2 );
    lua_pop( L, 1 );

    return proxy;
}
