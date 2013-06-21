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

void bindGorilla( lua_State* L )
{
    static LuaRef gorillaEnum = newTable( L );

    LuaRef gradient = newTable( L );
    gradient["NorthSouth"] = (int)Gradient_NorthSouth;
    gradient["WestEast"]   = (int)Gradient_WestEast;
    gradient["Diagonal"]   = (int)Gradient_Diagonal;
    gorillaEnum["Gradient"] = gradient;

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
        .addVariable( "Enum", &gorillaEnum )
        .beginClass<Silverback>( "Silverback" )
        .endClass()
        .endNamespace();

}
