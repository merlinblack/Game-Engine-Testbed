/*
-----------------------------------------------------------------------------

Copyright (c) 2010 Nigel Atkinson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
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
        luaL_error( L, "console.setVisible: expected boolean" );
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
        luaL_error( L, "console.setlogging: expected boolean" );
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
        luaL_error( L, "console.log: expected string" );
    }

    Ogre::LogManager::getSingletonPtr()->logMessage(lua_tostring( L, 1 ));
    return 0;
}

luaL_Reg consoleBindings[] =
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
    luaL_newlib( L, consoleBindings );
    lua_setglobal( L, "console" );
}

// Change console functions so that any remaining Lua code
// left to run, for example in finalises, does not explode.
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
