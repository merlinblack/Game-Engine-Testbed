#include <lua.hpp>
#include <LuaBridge.h>
#include <Gorilla.h>
#include <ward_ptr.h>

using namespace Gorilla;
using namespace luabridge;
using Ogre::Real;
using Ogre::Vector2;
using Ogre::ColourValue;
using Ogre::String;
using Ogre::MovableObject;

// Glue functions to create and destroy Gorilla elements.
// LuaBridge has been customised slightly to call luaL_error when a std::exception
// is caught when calling back into C++, so if a Lua script accesses a Gorilla element
// after destroying it, all you get is an Lua error rather than a crash.
//
ward_ptr<Gorilla::Rectangle> createRectangle( Layer *layer, Real x, Real y, Real width, Real height )
{
    return ward_ptr<Gorilla::Rectangle>( layer->createRectangle( x, y, width, height ) );
}

ward_ptr<Gorilla::Rectangle> createRectangle( Layer *layer, const Vector2& a, const Vector2& b )
{
    return layer->createRectangle( a, b );
}

void destroyRectangle( Layer *layer, ward_ptr<Gorilla::Rectangle> ptr )
{
    layer->destroyRectangle( ptr.get() );
    ptr.invalidate();
}

ward_ptr<Caption> createCaption( Layer* layer, Ogre::uint index, Real x, Real y, const String& text )
{
    return layer->createCaption( index, x, y, text );
}

void destroyCaption( Layer *layer, ward_ptr<Caption> ptr )
{
    layer->destroyCaption( ptr.get() );
    ptr.invalidate();
}

ward_ptr<MarkupText> createMarkupText( Layer *layer, Ogre::uint index, Real x, Real y, const String& text )
{
    return layer->createMarkupText( index, x, y, text );
}

void destroyMarkupText( Layer *layer, ward_ptr<MarkupText> ptr )
{
    layer->destroyMarkupText( ptr.get() );
    ptr.invalidate();
}

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

LuaRef makeTableReadonly( lua_State* L, LuaRef table )
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

void bindGorilla( lua_State* L )
{
    LuaRef gorillaEnum = newTable( L );

    LuaRef gradient = newTable( L );
    gradient["NorthSouth"] = (int)Gradient_NorthSouth;
    gradient["WestEast"]   = (int)Gradient_WestEast;
    gradient["Diagonal"]   = (int)Gradient_Diagonal;
    gorillaEnum["Gradient"] = makeTableReadonly( L, gradient );

    LuaRef border = newTable( L );
    border["North"] = (int)Border_North;
    border["East"]  = (int)Border_East;
    border["South"] = (int)Border_South;
    border["West"]  = (int)Border_West;
    gorillaEnum["Border"] = border;

    LuaRef corner = newTable( L );
    corner["TopLeft"]     = (int)TopLeft;
    corner["TopRight"]    = (int)TopRight;
    corner["BottomRight"] = (int)BottomRight;
    corner["BottomLeft"]  = (int)BottomLeft;
    gorillaEnum["Corner"] = corner;

    LuaRef textalign = newTable( L );
    textalign["Left"]   = (int)TextAlign_Left;
    textalign["Centre"] = (int)TextAlign_Centre;
    textalign["Right"]  = (int)TextAlign_Left;
    gorillaEnum["TextAlign"] = textalign;

    LuaRef vertalign = newTable( L );
    vertalign["Top"]    = (int)VerticalAlign_Top;
    vertalign["Middle"] = (int)VerticalAlign_Middle;
    vertalign["Bottom"] = (int)VerticalAlign_Bottom;
    gorillaEnum["VerticalAlign"] = vertalign;

    getGlobalNamespace( L )
        .beginNamespace( "Gorilla" )
        .beginClass<Silverback>( "Silverback" )
        .endClass()
        .endNamespace();

    LuaRef gorillaNS = getGlobal( L, "Gorilla" );

    gorillaNS["Enum"] = gorillaEnum;

}
