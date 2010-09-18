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

using namespace luabind;
using namespace Gorilla;
using Ogre::Real;
using Ogre::Vector2;
using Ogre::ColourValue;
using Ogre::String;

// Dummy class to hold enums.
class dummy {};

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
            class_<LayerContainer>( "LayerContainer" )
            .def( "createLayer", &LayerContainer::createLayer )
            .def( "destroy", &LayerContainer::destroy ),
            class_<Screen, LayerContainer>( "Screen" )
            .def_readonly( "width", &Screen::getWidth )
            .def_readonly( "height", &Screen::getHeight ),
            class_<ScreenRenderable, LayerContainer>( "ScreenRenderable" )
            .def( "getBoundingRadius", &ScreenRenderable::getBoundingRadius )
            .def( "getSquaredViewDepth", &ScreenRenderable::getSquaredViewDepth ),
            class_<Layer>( "Layer" )
            .property( "visible", &Layer::isVisible, &Layer::setVisible )
            .def( "show", &Layer::show )
            .def( "hide", &Layer::hide )
            .property( "alphaModifier", &Layer::getAlphaModifier, &Layer::setAlphaModifier )
            .def( "createRectangle", 
                    (Rectangle* (Layer::*)(Real, Real, Real, Real))&Layer::createRectangle )
            .def( "createRectangle",
                    (Rectangle* (Layer::*)(const Vector2&, const Vector2&))&Layer::createRectangle )
            .def( "destroyRectangle", &Layer::destroyRectangle )
            .def( "destroyAllRectangles", &Layer::destroyAllRectangles )
            // getRectangels - will need some sort of converter
            .def( "createPolygon", &Layer::createPolygon )
            .def( "destroyPolygon", &Layer::destroyPolygon )
            .def( "destroyAllPolygons", &Layer::destroyAllPolygons )
            // getPolygons
            .def( "createLineList", &Layer::createLineList )
            .def( "destroyLineList", &Layer::destroyLineList )
            .def( "destroyAllLineLists", &Layer::destroyAllLineLists )
            // getLineLists
            .def( "createQuadList", &Layer::createQuadList )
            .def( "destroyQuadList", &Layer::destroyQuadList )
            .def( "destroyAllQuadLists", &Layer::destroyAllQuadLists )
            // getQuadLists
            .def( "createCaption", &Layer::createCaption )
            .def( "destroyCaption", &Layer::destroyCaption )
            .def( "destroyAllCaptions", &Layer::destroyAllCaptions )
            // getCaptions
            .def( "createMarkupText", &Layer::createMarkupText )
            .def( "destroyMarkupText", &Layer::destroyMarkupText )
            .def( "destroyAllMarkupTexts", &Layer::destroyAllMarkupTexts )
            // getMarkupTexts
            ,
            class_<Rectangle>( "Rectangle" )
            .def( "intersects", &Rectangle::intersects )
            .def( "position", (Vector2 (Rectangle::*)() const )&Rectangle::position )
            .def( "position", (void (Rectangle::*)( const Real&, const Real& ))&Rectangle::position )
            .def( "position", (void (Rectangle::*)( const Vector2& ))&Rectangle::position )
            .property( "left", 
                    (Real (Rectangle::*)() const )&Rectangle::left,
                    (void (Rectangle::*)( const Real& ))&Rectangle::left )
            .property( "top", 
                    (Real (Rectangle::*)() const )&Rectangle::top,
                    (void (Rectangle::*)( const Real& ))&Rectangle::top )
            .property( "width", 
                    (Real (Rectangle::*)() const )&Rectangle::width,
                    (void (Rectangle::*)( const Real& ))&Rectangle::width )
            .property( "height", 
                    (Real (Rectangle::*)() const )&Rectangle::height,
                    (void (Rectangle::*)( const Real& ))&Rectangle::height )
            .def( "noBackground", &Rectangle::no_background )
            .def( "noBorder", &Rectangle::no_border )
            .def( "backgroundColour", 
                    (ColourValue (Rectangle::*)( QuadCorner ) const )&Rectangle::background_colour )
            .def( "backgroundColour",
                    (void (Rectangle::*)( const ColourValue& ))&Rectangle::background_colour )
            .def( "backgroundColour",
                    (void (Rectangle::*)( QuadCorner, const ColourValue& ))&Rectangle::background_colour )
            .def( "backgroundGradient", &Rectangle::background_gradient )
            .def( "backgroundImage",
                    (void (Rectangle::*)( const String& ))&Rectangle::background_image )
            .def( "borderColour",
                    (ColourValue (Rectangle::*)( Border ) const )&Rectangle::border_colour )
            .def( "borderColour",
                    (void (Rectangle::*)( const ColourValue& ))&Rectangle::border_colour )
            .def( "borderColour",
                    (void (Rectangle::*)( Border, const ColourValue& ))&Rectangle::border_colour )
            .property( "borderWidth",
                    (Real (Rectangle::*)() const )&Rectangle::border_width,
                    (void (Rectangle::*)( Real ))&Rectangle::border_width )
            .def( "border",
                    (void (Rectangle::*)( Real, const ColourValue& ))&Rectangle::border )
            .def( "border",
                    (void (Rectangle::*)( Real, const ColourValue&, const ColourValue&,
                                                const ColourValue&, const ColourValue& ))
                    &Rectangle::border )
        ]
    ];

    return;
}

