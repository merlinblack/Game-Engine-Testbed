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

#include "Gorilla.h"
#include <luabind/luabind.hpp>
#include "ward_ptr.h"

using namespace luabind;
using namespace Gorilla;
using Ogre::Real;
using Ogre::Vector2;
using Ogre::ColourValue;
using Ogre::String;
using Ogre::MovableObject;

// Dummy class to hold enums.
class dummy {};

// Glue create and destroy functions, to allow the use of ward pointers.

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

ward_ptr<Caption> createCaption( Layer* layer, Ogre::uint index, Real x, Real y, const Ogre::String& text )
{
    return layer->createCaption( index, x, y, text );
}

void destroyCaption( Layer *layer, ward_ptr<Caption> ptr )
{
    layer->destroyCaption( ptr.get() );
    ptr.invalidate();
}

ward_ptr<MarkupText> createMarkupText( Layer *layer, Ogre::uint index, Real x, Real y, const Ogre::String& text )
{
    return layer->createMarkupText( index, x, y, text );
}

void destroyMarkupText( Layer *layer, ward_ptr<MarkupText> ptr )
{
    layer->destroyMarkupText( ptr.get() );
    ptr.invalidate();
}

// The big binding!

void bindGorilla( lua_State *L )
{
    module(L)
    [
        // Bind everything within the "Gorilla" scope.
        namespace_( "Gorilla" )
        [
            class_<dummy>( "Gradient" )
            .enum_( "" )
            [
                value( "NorthSouth", Gradient_NorthSouth ),
                value( "WestEast", Gradient_WestEast ),
                value( "Diagonal", Gradient_Diagonal )
            ],
            class_<dummy>( "Border" )
            .enum_( "" )
            [
                value( "North", Border_North ),
                value( "South", Border_South ),
                value( "East", Border_East ),
                value( "West", Border_West )
            ],
            class_<dummy>( "QuadCorner" )
            .enum_( "" )
            [
                value( "TopLeft", TopLeft ),
                value( "TopRight", TopRight ),
                value( "BottomRight", BottomRight ),
                value( "BottomLeft", BottomLeft )
            ],
            class_<dummy>( "TextAlignment" )
            .enum_( "" )
            [
                value( "Left", TextAlign_Left ),
                value( "Right", TextAlign_Right ),
                value( "Centre", TextAlign_Centre )
            ],
            class_<dummy>( "VerticalAlignment" )
            .enum_( "" )
            [
                value( "Top", VerticalAlign_Top ),
                value( "Middle", VerticalAlign_Middle ),
                value( "Bottom", VerticalAlign_Bottom )
            ],
            class_<Silverback>( "Silverback" )
            // Singleton so no constructor, but a (static) getSingletonPtr instead.
            .scope
            [
                def( "getSingleton", &Silverback::getSingletonPtr )
            ]
            .def( "loadAtlas", &Silverback::loadAtlas )
            .def( "createScreen", &Silverback::createScreen )
            .def( "destroyScreen", &Silverback::destroyScreen )
            .def( "createScreenRenderable", &Silverback::createScreenRenderable )
            .def( "destroyScreenRenderable", &Silverback::destroyScreenRenderable ),
            class_<Screen>( "Screen" )
            .def( "createLayer", &Screen::createLayer )
            .def( "destroy", &Screen::destroy )
            .def_readonly( "width",  (float (Screen::*)())&Screen::getWidth )
            .def_readonly( "height", (float (Screen::*)())&Screen::getHeight ),
            class_<ScreenRenderable, MovableObject>( "ScreenRenderable" )
            .def( "createLayer", &ScreenRenderable::createLayer )
            .def( "destroy", &ScreenRenderable::destroy )
            .def( "getBoundingRadius", &ScreenRenderable::getBoundingRadius )
            .def( "getSquaredViewDepth", &ScreenRenderable::getSquaredViewDepth ),
            class_<Layer>( "Layer" )
            .property( "visible", &Layer::isVisible, &Layer::setVisible )
            .def( "show", &Layer::show )
            .def( "hide", &Layer::hide )
            .property( "alphaModifier", &Layer::getAlphaModifier, &Layer::setAlphaModifier )
            .def( "createRectangle", 
                    (ward_ptr<Gorilla::Rectangle>(*)(Layer*, Real, Real, Real, Real))&createRectangle )
            .def( "createRectangle",
                    (ward_ptr<Gorilla::Rectangle>(*)(Layer*, const Vector2&, const Vector2&))&createRectangle )
            .def( "destroyRectangle", &destroyRectangle )
            // getRectangels - will need some sort of converter
            .def( "createCaption", createCaption )
            .def( "destroyCaption", destroyCaption )
            // getCaptions
            .def( "createMarkupText", createMarkupText )
            .def( "destroyMarkupText", destroyMarkupText )
            // getMarkupTexts
            ,
            class_<Gorilla::Rectangle>( "Rectangle" )
            .def( "intersects", &Gorilla::Rectangle::intersects )
            .def( "position", (Vector2 (Gorilla::Rectangle::*)() const )&Gorilla::Rectangle::position )
            .def( "position", (void (Gorilla::Rectangle::*)( const Real&, const Real& ))&Gorilla::Rectangle::position )
            .def( "position", (void (Gorilla::Rectangle::*)( const Vector2& ))&Gorilla::Rectangle::position )
            .property( "left", 
                    (Real (Gorilla::Rectangle::*)() const )&Gorilla::Rectangle::left,
                    (void (Gorilla::Rectangle::*)( const Real& ))&Gorilla::Rectangle::left )
            .property( "top", 
                    (Real (Gorilla::Rectangle::*)() const )&Gorilla::Rectangle::top,
                    (void (Gorilla::Rectangle::*)( const Real& ))&Gorilla::Rectangle::top )
            .property( "width", 
                    (Real (Gorilla::Rectangle::*)() const )&Gorilla::Rectangle::width,
                    (void (Gorilla::Rectangle::*)( const Real& ))&Gorilla::Rectangle::width )
            .property( "height", 
                    (Real (Gorilla::Rectangle::*)() const )&Gorilla::Rectangle::height,
                    (void (Gorilla::Rectangle::*)( const Real& ))&Gorilla::Rectangle::height )
            .def( "noBackground", &Gorilla::Rectangle::no_background )
            .def( "noBorder", &Gorilla::Rectangle::no_border )
            .def( "backgroundColour", 
                    (ColourValue (Gorilla::Rectangle::*)( QuadCorner ) const )&Gorilla::Rectangle::background_colour )
            .def( "backgroundColour",
                    (void (Gorilla::Rectangle::*)( const ColourValue& ))&Gorilla::Rectangle::background_colour )
            .def( "backgroundColour",
                    (void (Gorilla::Rectangle::*)( QuadCorner, const ColourValue& ))&Gorilla::Rectangle::background_colour )
            .def( "backgroundGradient", &Gorilla::Rectangle::background_gradient )
            .def( "backgroundImage",
                    (void (Gorilla::Rectangle::*)( const String& ))&Gorilla::Rectangle::background_image )
            .def( "borderColour",
                    (ColourValue (Gorilla::Rectangle::*)( Border ) const )&Gorilla::Rectangle::border_colour )
            .def( "borderColour",
                    (void (Gorilla::Rectangle::*)( const ColourValue& ))&Gorilla::Rectangle::border_colour )
            .def( "borderColour",
                    (void (Gorilla::Rectangle::*)( Border, const ColourValue& ))&Gorilla::Rectangle::border_colour )
            .property( "borderWidth",
                    (Real (Gorilla::Rectangle::*)() const )&Gorilla::Rectangle::border_width,
                    (void (Gorilla::Rectangle::*)( Real ))&Gorilla::Rectangle::border_width )
            .def( "border",
                    (void (Gorilla::Rectangle::*)( Real, const ColourValue& ))&Gorilla::Rectangle::border )
            .def( "border",
                    (void (Gorilla::Rectangle::*)( Real, const ColourValue&, const ColourValue&,
                                                const ColourValue&, const ColourValue& ))
                    &Gorilla::Rectangle::border )
            ,
            class_<Caption>( "Caption" )
            .def( "intersects", &Caption::intersects )
            .property( "left",
                    (Real (Caption::*)() const)&Caption::left,
                    (void (Caption::*)( const Real& ))&Caption::left )
            .property( "top",
                    (Real (Caption::*)() const)&Caption::top,
                    (void (Caption::*)( const Real& ))&Caption::top )
            .property( "width",
                    (Real (Caption::*)() const)&Caption::width,
                    (void (Caption::*)( const Real& ))&Caption::width )
            .property( "height",
                    (Real (Caption::*)() const)&Caption::height,
                    (void (Caption::*)( const Real& ))&Caption::height )
            .def( "size", &Caption::size )
            .property( "text",
                    (String (Caption::*)() const)&Caption::text,
                    (void (Caption::*)( const String& ))&Caption::text )
            .property( "align",
                    (TextAlignment (Caption::*)() const )&Caption::align,
                    (void (Caption::*)( const TextAlignment& ))&Caption::align )
            .property( "verticalAlign",
                    (VerticalAlignment (Caption::*)() const )&Caption::vertical_align,
                    (void (Caption::*)( const VerticalAlignment& ))&Caption::vertical_align )
            .property( "colour",
                    (ColourValue (Caption::*)() const )&Caption::colour,
                    (void (Caption::*)( const ColourValue& ))&Caption::colour )
            .property( "background",
                    (ColourValue (Caption::*)() const )&Caption::background,
                    (void (Caption::*)( const ColourValue& ))&Caption::background )
            .def( "noBackground", &Caption::no_background )
            , 
            class_<MarkupText>( "MarkupText" )
            .property( "maxTextWidth", &MarkupText::maxTextWidth )
            .property( "left",
                    (Real (MarkupText::*)() const)&MarkupText::left,
                    (void (MarkupText::*)( const Real& ))&MarkupText::left )
            .property( "top",
                    (Real (MarkupText::*)() const)&MarkupText::top,
                    (void (MarkupText::*)( const Real& ))&MarkupText::top )
            //.property( "width",
            //        (Real (MarkupText::*)() const)&MarkupText::width,
            //        (void (MarkupText::*)( const Real& ))&MarkupText::width )
            //.property( "height",
            //       (Real (MarkupText::*)() const)&MarkupText::height,
            //        (void (MarkupText::*)( const Real& ))&MarkupText::height )
            //.def( "size", &MarkupText::size )
            .property( "text",
                    (String (MarkupText::*)() const)&MarkupText::text,
                    (void (MarkupText::*)( const String& ))&MarkupText::text )
            //.property( "background",
            //        (ColourValue (MarkupText::*)() const )&MarkupText::background,
            //        (void (MarkupText::*)( const ColourValue& ))&MarkupText::background )
        ]
    ];

    return;
}

