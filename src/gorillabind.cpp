#include <lua.hpp>
#include <LuaBridge.h>
#include <luareadonlytable.h>
#include <Gorilla.h>

using namespace Gorilla;
using namespace luabridge;
using Ogre::Real;
using Ogre::Vector2;
using Ogre::ColourValue;
using Ogre::String;
using Ogre::MovableObject;

void Caption_align_set( Caption* ptr, int alignment ) // LuaBridge does not handle enums
{
    ptr->align( (TextAlignment) alignment );
}

int Caption_align_get( const Caption* ptr )
{
    return (int)ptr->align();
}

void Caption_valign_set( Caption* ptr, int valignment )
{
    ptr->vertical_align( (VerticalAlignment) valignment );
}

int Caption_valign_get( const Caption* ptr )
{
    return (int)ptr->vertical_align();
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
        .addFunction( "createRectangle", (Gorilla::Rectangle* (Layer::*)(Real,Real,Real,Real))&Layer::createRectangle )
        .addFunction( "createRectangleV2", (Gorilla::Rectangle* (Layer::*)(const Vector2&, const Vector2& ))&Layer::createRectangle )
        .addFunction( "destroyRectangle", &Layer::destroyRectangle )
        .addFunction( "createCaption", &Layer::createCaption )
        .addFunction( "destroyCaption", &Layer::destroyCaption )
        .addFunction( "createMarkupText", &Layer::createMarkupText )
        .addFunction( "destroyMarkupText", &Layer::destroyMarkupText )
        .endClass()
        .beginClass<Gorilla::Rectangle>( "Rectangle" )
        .addFunction( "intersects", &Rectangle::intersects )
        .addFunction( "getPosition", (Vector2 (Rectangle::*)() const )&Rectangle::position )
        .addFunction( "setPosition", (void (Rectangle::*)( const Real&, const Real& ))&Rectangle::position )
        .addFunction( "setPositionV2", (void (Rectangle::*)( const Vector2& ))&Rectangle::position )
        .addProperty( "left", (Real (Rectangle::*)() const )&Rectangle::left, (void (Rectangle::*)( const Real& ))&Rectangle::left )
        .addProperty( "top", (Real (Rectangle::*)() const )&Rectangle::top, (void (Rectangle::*)( const Real& ))&Rectangle::top )
        .addProperty( "width", (Real (Rectangle::*)() const )&Rectangle::width, (void (Rectangle::*)( const Real& ))&Rectangle::width )
        .addProperty( "height", (Real (Rectangle::*)() const )&Rectangle::height, (void (Rectangle::*)( const Real& ))&Rectangle::height )
        .addFunction( "noBackground", &Rectangle::no_background )
        .addFunction( "noBorder", &Rectangle::no_border )
        // These funcs with QuadCorner and Border will need a multiplex function, not only to call the right function,
        // but to understand and cast the enums.
        //.addFunction( "getBackgroundColour", (ColourValue (Rectangle::*)( QuadCorner ) const )&Gorilla::Rectangle::background_colour )
        .addFunction( "backgroundColour", (void (Rectangle::*)( const ColourValue& ))&Rectangle::background_colour )
        //.addFunction( "backgroundColour", (void (Rectangle::*)( QuadCorner, const ColourValue& ))&Rectangle::background_colour )
        .addFunction( "backgroundGradient", &Rectangle::background_gradient )
        .addFunction( "backgroundImage", (void (Rectangle::*)( const String& ))&Rectangle::background_image )
        //.addFunction( "borderColour", (ColourValue (Rectangle::*)( Border ) const )&Rectangle::border_colour )
        //.addFunction( "borderColour", (void (Rectangle::*)( const ColourValue& ))&Rectangle::border_colour )
        //.addFunction( "borderColour", (void (Rectangle::*)( Border, const ColourValue& ))&Rectangle::border_colour )
        .addProperty( "borderWidth", (Real (Rectangle::*)() const )&Rectangle::border_width, (void (Rectangle::*)( Real ))&Rectangle::border_width )
        //.addFunction( "border", (void (Rectangle::*)( Real, const ColourValue& ))&Rectangle::border )
        .addFunction( "border", (void (Rectangle::*)( Real, const ColourValue&, const ColourValue&, const ColourValue&, const ColourValue& )) &Rectangle::border )
        .endClass()
        .beginClass<Caption>( "Caption" )
        .addFunction( "intersects", &Caption::intersects )
        .addProperty( "left", (Real (Caption::*)() const)&Caption::left, (void (Caption::*)( const Real& ))&Caption::left )
        .addProperty( "top", (Real (Caption::*)() const)&Caption::top, (void (Caption::*)( const Real& ))&Caption::top )
        .addProperty( "width", (Real (Caption::*)() const)&Caption::width, (void (Caption::*)( const Real& ))&Caption::width )
        .addProperty( "height", (Real (Caption::*)() const)&Caption::height, (void (Caption::*)( const Real& ))&Caption::height )
        .addFunction( "size", &Caption::size )
        .addProperty( "text", (String (Caption::*)() const)&Caption::text, (void (Caption::*)( const String& ))&Caption::text )
        .addFunction( "getAlign", &Caption_align_get )
        .addFunction( "setAlign", &Caption_align_set )
        .addFunction( "getVerticalAlign", &Caption_valign_get )
        .addFunction( "setVerticalAlign", &Caption_valign_set )
        .addProperty( "colour", (ColourValue (Caption::*)() const )&Caption::colour, (void (Caption::*)( const ColourValue& ))&Caption::colour )
        .addProperty( "background", (ColourValue (Caption::*)() const )&Caption::background, (void (Caption::*)( const ColourValue& ))&Caption::background )
        .addFunction( "noBackground", &Caption::no_background )
        .endClass()
        .beginClass<MarkupText>( "MarkupText" )
        .addProperty( "maxTextWidth", (Real (MarkupText::*)() const)&MarkupText::maxTextWidth )
        .addProperty( "left", (Real (MarkupText::*)() const)&MarkupText::left, (void (MarkupText::*)( const Real& ))&MarkupText::left )
        .addProperty( "top", (Real (MarkupText::*)() const)&MarkupText::top, (void (MarkupText::*)( const Real& ))&MarkupText::top )
        .addProperty( "text", (String (MarkupText::*)() const)&MarkupText::text, (void (MarkupText::*)( const String& ))&MarkupText::text )
        .endClass()
        .endNamespace();

    LuaRef gorillaNS = getGlobal( L, "Gorilla" );

    gorillaNS["Enum"] = makeReadonlyProxy( gorillaEnum );

}
