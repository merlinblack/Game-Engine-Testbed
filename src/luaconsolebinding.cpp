#include "luaconsole.h"
#include <OgreLogManager.h>
#include <lua.hpp>

// Lua bindings to control the console.

int LuaConsole_isVisible( lua_State *L )
{
    if( lua_gettop( L ) )
    {
        lua_pushliteral( L, "isVisible does not take any parameters." );
        lua_error( L );
    }

    lua_pushboolean( L, LuaConsole::getSingleton().isVisible() );

    return 1;
}

int LuaConsole_setVisible( lua_State *L )
{
    if( ! lua_isboolean( L, 1 ) )
    {
        luaL_typerror( L, 1, "boolean" );
    }

    LuaConsole::getSingleton().setVisible( lua_toboolean( L, 1 ) );

    return 0;
}

int LuaConsole_isLogging( lua_State *L )
{
    if( lua_gettop( L ) )
    {
        lua_pushliteral( L, "isLogging does not take any parameters." );
        lua_error( L );
    }

    lua_pushboolean( L, LuaConsole::getSingleton().isLogging() );

    return 1;
}

int LuaConsole_setLogging( lua_State *L )
{
    if( ! lua_isboolean( L, 1 ) )
    {
        luaL_typerror( L, 1, "boolean" );
    }

    LuaConsole::getSingleton().setLogging( lua_toboolean( L, 1 ) );

    return 0;
}

int LuaConsole_print( lua_State *L )
{
    int i;
    int nArgs = lua_gettop(L);
    std::string output;

    lua_getglobal( L, "tostring" );

    for( i = 1; i <= nArgs; i++ )
    {
        const char *s;
        lua_pushvalue( L, -1 );
        lua_pushvalue( L, i );
        lua_pcall( L, 1, 1, 0 );
        s = lua_tostring( L, -1 );
        if( ! s )
            return luaL_error( L, LUA_QL("tostring") " must return a string to ", LUA_QL("print"));

        output += s;

        if( i < nArgs )
            output += "\t";

        lua_pop( L, 1 );    // Pop return value of "tostring"
    }

    LuaConsole::getSingleton().print( output );

    return 0;
}

int LuaConsole_clearHistory( lua_State *L )
{
    if( lua_gettop( L ) )
    {
        lua_pushliteral( L, "clearHistory does not take any parameters." );
        lua_error( L );
    }

    LuaConsole::getSingleton().clearHistory();

    return 0;
}

int LuaConsole_clearConsole( lua_State *L )
{
    if( lua_gettop( L ) )
    {
        lua_pushliteral( L, "clear does not take any parameters." );
        lua_error( L );
    }

    LuaConsole::getSingleton().clearConsole();

    return 0;
}

int LuaConsole_log( lua_State *L )
{
    if( ! lua_isstring( L, 1 ) )
    {
        luaL_typerror( L, 1, "string" );
    }

    Ogre::LogManager::getSingletonPtr()->logMessage(lua_tostring( L, 1 ));
    return 0;
}

luaL_reg consoleBindings[] =
{
    { "isVisible", LuaConsole_isVisible },
    { "setVisible", LuaConsole_setVisible },
    { "isLogging", LuaConsole_isLogging },
    { "setLogging", LuaConsole_setLogging },
    { "print", LuaConsole_print },
    { "clearHistory", LuaConsole_clearHistory },
    { "clear", LuaConsole_clearConsole },
    { "log", LuaConsole_log },
    { NULL, NULL }
};

// Load bindings into Lua
void bindLuaConsole( lua_State *L )
{
    luaL_register( L, "console", consoleBindings );
    lua_pop( L, 1 );
}

// Change console functions so that any remaining Lua code
// left to run, for example in finalisers, does not explode.
void unBindConsole( lua_State *L )
{
    luaL_dostring( L, "console.print = console.log "
                   "function console.isVisible() return false end "
                   "function console.setVisible() return end "
                   "function console.isLogging() return false end "
                   "function console.setLogging() return end "
                   "function console.clearHistory() return end "
                   "function console.clear() return end " );
}
