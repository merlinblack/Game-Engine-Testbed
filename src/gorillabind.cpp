#include <lua.hpp>
#include <LuaBridge.h>
#include <luareadonlytable.h>
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
ward_ptr<Rectangle> createRectangle( Layer *layer, Real x, Real y, Real width, Real height )
{
    return ward_ptr<Gorilla::Rectangle>( layer->createRectangle( x, y, width, height ) );
}

ward_ptr<Rectangle> createRectangle( Layer *layer, const Vector2& a, const Vector2& b )
{
    return layer->createRectangle( a, b );
}

void destroyRectangle( Layer *layer, ward_ptr<Rectangle> ptr )
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
    LuaRef gorillaEnum = newTable( L );

    LuaRef gradient = newTable( L );
    gradient["NorthSouth"] = (int)Gradient_NorthSouth;
    gradient["WestEast"]   = (int)Gradient_WestEast;
    gradient["Diagonal"]   = (int)Gradient_Diagonal;
    gorillaEnum["Gradient"] = makeReadonlyProxy( gradient );

    LuaRef border = newTable( L );
    border["North"] = (int)Border_North;
    border["East"]  = (int)Border_East;
    border["South"] = (int)Border_South;
    border["West"]  = (int)Border_West;
    gorillaEnum["Border"] = makeReadonlyProxy( border);

    LuaRef corner = newTable( L );
    corner["TopLeft"]     = (int)TopLeft;
    corner["TopRight"]    = (int)TopRight;
    corner["BottomRight"] = (int)BottomRight;
    corner["BottomLeft"]  = (int)BottomLeft;
    gorillaEnum["Corner"] = makeReadonlyProxy( corner );

    LuaRef textalign = newTable( L );
    textalign["Left"]   = (int)TextAlign_Left;
    textalign["Centre"] = (int)TextAlign_Centre;
    textalign["Right"]  = (int)TextAlign_Right;
    gorillaEnum["TextAlign"] = makeReadonlyProxy( textalign );

    LuaRef vertalign = newTable( L );
    vertalign["Top"]    = (int)VerticalAlign_Top;
    vertalign["Middle"] = (int)VerticalAlign_Middle;
    vertalign["Bottom"] = (int)VerticalAlign_Bottom;
    gorillaEnum["VerticalAlign"] = makeReadonlyProxy( vertalign );

    getGlobalNamespace( L )
        .beginNamespace( "Gorilla" )
        .beginClass<Silverback>( "Silverback" )
        .addStaticFunction( "getSingleton", &Silverback::getSingletonPtr )
        .addFunction( "loadAtlas",    &Silverback::loadAtlas )
        .addFunction( "createScreen", &Silverback::createScreen )
        .addFunction( "createScreenRenderable",  &Silverback::createScreenRenderable )
        .addFunction( "destroyScreenRenderable", &Silverback::destroyScreenRenderable )
        .endClass()
        .beginClass<LayerContainer>("LayerContainer")
        .endClass()
        .deriveClass<Screen,LayerContainer>("Screen")
        .addFunction( "createLayer", &Screen::createLayer )
        .addFunction( "destroy", &Screen::destroy )
        .addProperty( "width", &Screen::getWidth )
        .addProperty( "height", &Screen::getHeight )
        .endClass()
        // TODO - ScreenRenderable once Ogre::MovableObject is done
        .beginClass<Layer>("Layer")
        .addProperty( "visible", &Layer::isVisible, &Layer::setVisible )
        .addFunction( "show", &Layer::show )
        .addFunction( "hide", &Layer::hide )
        .addProperty( "alphaModifier", &Layer::getAlphaModifier, &Layer::setAlphaModifier )
        .addFunction( "createRectangle", (ward_ptr<Rectangle>(*)(Layer*,Real,Real,Real,Real))&createRectangle )
        // May need other one that takes two Vector2?
        .addFunction( "destroyRectangle", &destroyRectangle )
        // TODO create and destroy for Captions and Markup Text
        .endClass()
        .beginClass<Rectangle>( "Rectangle" )
        .addFunction( "intersects", &Rectangle::intersects )
        //.addFunction( "position", (Vector2 (Rectangle::*)() const )&Rectangle::position )
        .addFunction( "position", (void (Rectangle::*)( const Real&, const Real& ))&Rectangle::position )
        //.addFunction( "position", (void (Rectangle::*)( const Vector2& ))&Rectangle::position )
        .addProperty( "left", (Real (Rectangle::*)() const )&Rectangle::left, (void (Rectangle::*)( const Real& ))&Rectangle::left )
        .addProperty( "top", (Real (Rectangle::*)() const )&Rectangle::top, (void (Rectangle::*)( const Real& ))&Rectangle::top )
        .addProperty( "width", (Real (Rectangle::*)() const )&Rectangle::width, (void (Rectangle::*)( const Real& ))&Rectangle::width )
        .addProperty( "height", (Real (Rectangle::*)() const )&Rectangle::height, (void (Rectangle::*)( const Real& ))&Rectangle::height )
        .addFunction( "noBackground", &Rectangle::no_background )
        .addFunction( "noBorder", &Rectangle::no_border )
        //.addFunction( "backgroundColour", (ColourValue (Rectangle::*)( QuadCorner ) const )&Gorilla::Rectangle::background_colour )
        .addFunction( "backgroundColour", (void (Rectangle::*)( const ColourValue& ))&Rectangle::background_colour )
        //.addFunction( "backgroundColour", (void (Rectangle::*)( QuadCorner, const ColourValue& ))&Rectangle::background_colour )
        .addFunction( "backgroundGradient", &Rectangle::background_gradient )
        .addFunction( "backgroundImage", (void (Rectangle::*)( const String& ))&Rectangle::background_image )
        //.addFunction( "borderColour", (ColourValue (Rectangle::*)( Border ) const )&Rectangle::border_colour )
        //.addFunction( "borderColour", (void (Rectangle::*)( const ColourValue& ))&Rectangle::border_colour )
        .addFunction( "borderColour", (void (Rectangle::*)( Border, const ColourValue& ))&Rectangle::border_colour )
        .addProperty( "borderWidth", (Real (Rectangle::*)() const )&Rectangle::border_width, (void (Rectangle::*)( Real ))&Rectangle::border_width )
        //.addFunction( "border", (void (Rectangle::*)( Real, const ColourValue& ))&Rectangle::border )
        .addFunction( "border", (void (Rectangle::*)( Real, const ColourValue&, const ColourValue&, const ColourValue&, const ColourValue& )) &Rectangle::border )
        .endClass()
        .endNamespace();

    LuaRef gorillaNS = getGlobal( L, "Gorilla" );

    gorillaNS["Enum"] = makeReadonlyProxy( gorillaEnum );

}
