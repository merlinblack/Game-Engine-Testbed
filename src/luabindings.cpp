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
#include <Ogre.h>
#include <luabind/luabind.hpp>

// Prototype these before operator.hpp so it can be found for tostring() operator.
std::ostream& operator<<( std::ostream& stream, const Ogre::Entity& ent );
std::ostream& operator<<( std::ostream& stream, const Ogre::SceneNode& node );

#include <luabind/operator.hpp>

using namespace luabind;
using namespace Ogre;

// Some helpful macros for defining constants (sort of) in Lua.  Similar to this code:
// object g = globals(L);
// object table = g["class"];
// table["constant"] = class::constant;

#define LUA_STATIC_START( class ) { object g = globals(L); object table = g[#class];
#define LUA_STATIC( class, name ) table[#name] = class::name
#define LUA_STATIC_END }

void AddChild( OverlayElement* element, OverlayElement* child )
{
    OverlayContainer* container = dynamic_cast<OverlayContainer*>(element);

    if( container )
    {
        if( child )
            container->addChild( child );
        else
            LogManager::getSingleton().stream() << "Whoops! nil child given.";
    }
    else
    {
        LogManager::getSingleton().stream() << "Element is not a container type.";
    }
}

OverlayManager* GetOverlayManager()
{
    return OverlayManager::getSingletonPtr();
}

void ElementHelp( OverlayElement* element )
{
    ParameterList list = element->getParameters();

    ParameterList::iterator iter;

    for( iter = list.begin(); iter != list.end(); iter++ )
    {
        LogManager::getSingleton().stream() << iter->name << "\t" << iter->description << "\n";
    }
}

void OverlayAdd2D( Overlay* overlay, OverlayElement* element )
{
    OverlayContainer* container = dynamic_cast<OverlayContainer*>(element);

    if( container )
    {
        overlay->add2D( container );
    }
    else
    {
        LogManager::getSingleton().stream() << "Element is not a container type.";
    }
}

void DestroyOverlay( Overlay* overlay )
{
    //LogManager::getSingleton().stream() << "Destroying overlay: " << overlay->getName();
    OverlayManager::getSingleton().destroy( overlay );
}

void DestroyOverlayElement( OverlayElement* element )
{
    //LogManager::getSingleton().stream() << "Destroying overlay element: " << element->getName();
    OverlayManager::getSingleton().destroyOverlayElement( element );
}

String tostringOverlay( Overlay* overlay )
{
    return "Overlay: " + overlay->getName();
}

String tostringOverlayElement( OverlayElement* element )
{
    return "Overlay Element: " + element->getName();
}

class OverlayElementWrapper : public OverlayElement, public wrap_base
{
};

class OverlayWrapper : public Overlay, public wrap_base
{
};

SceneNode* getRootSceneNode()
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    return sceneManager->getRootSceneNode();
}

bool sceneNodeExists( const String name )
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    return sceneManager->hasSceneNode( name );
}

SceneNode *getSceneNode( const String name )
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    return sceneManager->getSceneNode( name );
}

Camera* getCamera()
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    return sceneManager->getCamera("MainCamera");
}

Entity* createEntity( String name, String meshfile )
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    return sceneManager->createEntity( name, meshfile );
}

void destroyEntity( String name )
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    sceneManager->destroyEntity( name );
}

void bindGui( lua_State *L )
{
    module(L)
    [
        class_<Overlay, OverlayWrapper>("OgreOverlay")
        .def("show", &Overlay::show )
        .def("hide", &Overlay::hide )
        .def("setZOrder", &Overlay::setZOrder )
        .def("add2D", &OverlayAdd2D )
        .def("__finalize", DestroyOverlay )
        .def("__tostring", tostringOverlay )
        ,

        class_<OverlayElement, OverlayElementWrapper>("OverlayElement")
        .def("setPosition", &OverlayElement::setPosition )
        .def("setDimensions", &OverlayElement::setDimensions )
        .def("setMaterialName", &OverlayElement::setMaterialName )
        .def("setParameter", &OverlayElement::setParameter )
        .def("addChild", &AddChild )
        .def("contains", &OverlayElement::contains )
        .def("help", &ElementHelp )
        .def("__finalize", DestroyOverlayElement )
        .def("__tostring", tostringOverlayElement )
        ,

        class_<OverlayManager>("OverlayManager")
        .def("createOverlay", &OverlayManager::create )
        .def("createElement", &OverlayManager::createOverlayElement )
        ,
        def("getOverlayManager", &GetOverlayManager )
    ];
}

void bindQuaternion( lua_State* L )
{
    module(L)
    [
        class_<Quaternion>( "Quaternion" )
        .def(tostring(self))
        .def_readwrite( "x", &Quaternion::x )
        .def_readwrite( "y", &Quaternion::y )
        .def_readwrite( "z", &Quaternion::z )
        .def_readwrite( "w", &Quaternion::w )
        .def(constructor<>())
        .def(constructor<Real,Real,Real,Real>())
        .def(constructor<const Radian&, const Vector3& >())
        .def("dot", &Quaternion::Dot )

        // Operators
        .def( self + other<Quaternion>() )
        .def( self - other<Quaternion>() )
        .def( self * other<Quaternion>() )
        .def( self * Real() )
        .def( self * other<Vector3>() )
    ];

    LUA_STATIC_START( Quaternion )
        LUA_STATIC( Quaternion, ZERO );
        LUA_STATIC( Quaternion, IDENTITY );
    LUA_STATIC_END;
}

void bindVector3( lua_State* L )
{
    module(L)
    [
        class_<Vector3>( "Vector3" )
        .def(tostring(self))
        .def_readwrite( "x", &Vector3::x )
        .def_readwrite( "y", &Vector3::y )
        .def_readwrite( "z", &Vector3::z )
        .def(constructor<>())
        .def(constructor<Vector3&>())
        .def(constructor<Real, Real, Real>())
        .def("absDotProduct", &Vector3::absDotProduct)
        .def("crossProduct", &Vector3::crossProduct )
        .def("directionEquals", &Vector3::directionEquals )
        .def("distance", &Vector3::distance )
        .def("dotProduct", &Vector3::dotProduct )
        .def("getRotationTo", &Vector3::getRotationTo )
        .def("isZeroLength", &Vector3::isZeroLength )
        .def("length", &Vector3::length )
        .def("makeCeil", &Vector3::makeCeil )
        .def("makeFloor", &Vector3::makeFloor )
        .def("midPoint", &Vector3::midPoint )
        .def("normalise", &Vector3::normalise )
        .def("normalisedCopy", &Vector3::normalisedCopy )
        .def("perpendicular", &Vector3::perpendicular )
        .def("positionCloses", &Vector3::positionCloses )
        .def("positionEquals", &Vector3::positionEquals )
        //.def("ptr", &Vector3::ptr )
        .def("randomDeviant", &Vector3::randomDeviant )
        .def("reflect", &Vector3::reflect )
        .def("squaredDistance", &Vector3::squaredDistance )
        .def("squaredLength", &Vector3::squaredLength )
        .def("angleBetween", &Vector3::angleBetween )

        // Operators

        .def( self + other<Vector3>() )
        .def( self - other<Vector3>() )
        .def( self * other<Vector3>() )
        .def( self * Real() )
    ];

    LUA_STATIC_START( Vector3 )
        LUA_STATIC( Vector3, ZERO);
        LUA_STATIC( Vector3, UNIT_X);
        LUA_STATIC( Vector3, UNIT_Y);
        LUA_STATIC( Vector3, UNIT_Z);
        LUA_STATIC( Vector3, NEGATIVE_UNIT_X);
        LUA_STATIC( Vector3, NEGATIVE_UNIT_Y);
        LUA_STATIC( Vector3, NEGATIVE_UNIT_Z);
        LUA_STATIC( Vector3, UNIT_SCALE);
    LUA_STATIC_END;
}

void bindVector2( lua_State* L )
{
    module(L)
    [
        class_<Vector2>( "Vector2" )
        .def(tostring(self))
        .def_readwrite( "x", &Vector2::x )
        .def_readwrite( "y", &Vector2::y )
        .def(constructor<>())
        .def(constructor<Vector2&>())
        .def(constructor<Real, Real>())
        .def("crossProduct", &Vector2::crossProduct )
        .def("dotProduct", &Vector2::dotProduct )
        .def("isZeroLength", &Vector2::isZeroLength )
        .def("length", &Vector2::length )
        .def("makeCeil", &Vector2::makeCeil )
        .def("makeFloor", &Vector2::makeFloor )
        .def("midPoint", &Vector2::midPoint )
        .def("normalise", &Vector2::normalise )
        .def("normalisedCopy", &Vector2::normalisedCopy )
        .def("perpendicular", &Vector2::perpendicular )
        .def("randomDeviant", &Vector2::randomDeviant )
        .def("reflect", &Vector2::reflect )
        .def("squaredLength", &Vector2::squaredLength )

        // Operators

        .def( self + other<Vector2>() )
        .def( self - other<Vector2>() )
        .def( self * other<Vector2>() )
        .def( self * Real() )
    ];

    LUA_STATIC_START( Vector2 )
        LUA_STATIC( Vector2, ZERO);
        LUA_STATIC( Vector2, UNIT_X);
        LUA_STATIC( Vector2, UNIT_Y);
        LUA_STATIC( Vector2, NEGATIVE_UNIT_X);
        LUA_STATIC( Vector2, NEGATIVE_UNIT_Y);
        LUA_STATIC( Vector2, UNIT_SCALE);
    LUA_STATIC_END;
}

void bindColourValue( lua_State* L )
{
    module(L)
    [
        class_<ColourValue>("ColourValue")
        .def(constructor<>())
        .def(constructor<Real, Real, Real, Real>())
        .def(constructor<Real, Real, Real>())
        .def(tostring(self))
        .def_readwrite( "r", &ColourValue::r)
        .def_readwrite( "g", &ColourValue::g )
        .def_readwrite( "b", &ColourValue::b )
        .def_readwrite( "a", &ColourValue::a )
        .def( "saturate", &ColourValue::saturate )

        // Operators

        .def( self + other<ColourValue>() )
        .def( self - other<ColourValue>() )
        .def( self * other<ColourValue>() )
        .def( self * Real() )
        .def( self / Real() )
        .def(tostring(self))
    ];

    LUA_STATIC_START( ColourValue )
        LUA_STATIC( ColourValue, ZERO);
        LUA_STATIC( ColourValue, Black);
        LUA_STATIC( ColourValue, White);
        LUA_STATIC( ColourValue, Red);
        LUA_STATIC( ColourValue, Green);
        LUA_STATIC( ColourValue, Blue);
    LUA_STATIC_END;
}

std::ostream& operator<<( std::ostream& stream, const Entity& ent )
{
    return stream << ent.getName();
}

void bindEntity( lua_State* L ) // And Movable Object for now.
{
    module(L)
    [
        class_<MovableObject>("MovableObject"),
        class_<Entity, MovableObject>("Entity")
        .def(tostring(self))
        .def("setMaterialName", &Entity::setMaterialName )
        .def("setDisplaySkeleton", &Entity::setDisplaySkeleton )
    ];
}

// Fake member function for simplifying binding, as the real functions
// have optional arguments, which I don't want to use in the Lua script.
// However luabind does not support optional arguments.
// Think of "obj" as "this"
SceneNode *createChildSceneNode( SceneNode *obj, const String name )
{
    return obj->createChildSceneNode( name );
}

void SceneNode_destroy( SceneNode *obj )
{
    Root* root = Root::getSingletonPtr();
    SceneManager* sceneManager = root->getSceneManager("SceneManagerInstance");

    sceneManager->destroySceneNode( obj );
}

void SceneNode_yaw( SceneNode *obj, const Real degrees )
{
    return obj->yaw( Degree( degrees ) );
}

void SceneNode_pitch( SceneNode *obj, const Real degrees )
{
    return obj->pitch( Degree( degrees ) );
}

void SceneNode_roll( SceneNode *obj, const Real degrees )
{
    return obj->roll( Degree( degrees ) );
}

void SceneNode_rotate( SceneNode *obj, const Quaternion& q )
{
    return obj->rotate( q );
}

void SceneNode_translate( SceneNode *obj, const Vector3& v )
{
    return obj->translate( v );
}

Vector3 SceneNode_getPosition( SceneNode* obj )
{
    // This function returns a reference which confuses luabind.
    // So we wrap it.
    return obj->getPosition();
}

Quaternion SceneNode_getOrientation( SceneNode* obj )
{
    // This function returns a reference which confuses luabind.
    // So we wrap it.
    return obj->getOrientation();
}

std::ostream& operator<<( std::ostream& stream, const SceneNode& node )
{
    return stream << node.getName();
}

void bindSceneNode( lua_State* L )
{
    module(L)
    [
        class_<SceneNode>("SceneNode")
        .def("createChildSceneNode", &createChildSceneNode )
        .def("attachObject", &SceneNode::attachObject )
        .def("yaw", SceneNode_yaw )
        .def("pitch", SceneNode_pitch )
        .def("roll", SceneNode_roll )
        .def("setPosition", (void( SceneNode::*)(const Vector3&))&SceneNode::setPosition )
        .def("setPosition", (void( SceneNode::*)(Real,Real,Real))&SceneNode::setPosition )
        .def("getPosition", &SceneNode_getPosition )
        .def("setOrientation", (void( SceneNode::*)( const Quaternion& ))&SceneNode::setOrientation )
        .def("getOrientation", &SceneNode_getOrientation )
        .def("scale", (void( SceneNode::*)(Real,Real,Real))&SceneNode::scale )
        .def("scale", (void( SceneNode::*)(const Vector3&))&SceneNode::scale )
        .def("showBoundingBox", &SceneNode::showBoundingBox )
        .def("rotate", &SceneNode_rotate )
        .def("translate", &SceneNode_translate )
        .def("translate", (void( SceneNode::*)(const Vector3&, SceneNode::TransformSpace))&SceneNode::translate )
        .def("destroy", &SceneNode_destroy )
        .def(tostring(self))
    ];

    LUA_STATIC_START( SceneNode )
        LUA_STATIC( SceneNode, TS_LOCAL );
        LUA_STATIC( SceneNode, TS_PARENT );
        LUA_STATIC( SceneNode, TS_WORLD );
    LUA_STATIC_END;
}

void bindCamera( lua_State* L )
{
    module(L)
    [
        class_<Camera, MovableObject>("Camera")
        .def("setPosition", (void( Camera::*)(const Vector3&))&Camera::setPosition )
        .def("setPosition", (void( Camera::*)(Real,Real,Real))&Camera::setPosition )
        .def("lookAt", (void( Camera::*)(const Vector3&))&Camera::lookAt )
        .def("lookAt", (void( Camera::*)(Real,Real,Real))&Camera::lookAt )
        .def("setNearClipDistance", &Camera::setNearClipDistance )
        .def("setFarClipDistance", &Camera::setFarClipDistance )
        .def(tostring(self))
    ];
}

void bindRadian( lua_State* L )
{
    module(L)
    [
        class_<Radian>("Radian")
        .def(constructor< Real >() )
        .def(constructor< Degree& >() )
        .def(tostring(self))
        .def_readonly( "rad", &Radian::valueRadians )
        .def_readonly( "deg", &Radian::valueDegrees ),

        class_<Degree>("Degree")
        .def(constructor< Real >() )
        .def(constructor< Radian& >() )
        .def(tostring(self))
        .def_readonly( "rad", &Degree::valueRadians )
        .def_readonly( "deg", &Degree::valueDegrees )
    ];
}

RenderWindow::FrameStats getFrameStats()
{
    Root* root = Root::getSingletonPtr();
    RenderWindow* window = root->getAutoCreatedWindow();

    return window->getStatistics();
}

void bindFrameStats( lua_State* L )
{
    module(L)
    [
        class_<RenderWindow::FrameStats>( "FrameStats" )
        .def_readonly( "lastFPS", &RenderWindow::FrameStats::lastFPS )
        .def_readonly( "avgFPS", &RenderWindow::FrameStats::avgFPS )
        .def_readonly( "bestFPS", &RenderWindow::FrameStats::bestFPS )
        .def_readonly( "worstFPS", &RenderWindow::FrameStats::worstFPS )
        .def_readonly( "bestFrameTime", &RenderWindow::FrameStats::bestFrameTime )
        .def_readonly( "worstFrameTime", &RenderWindow::FrameStats::worstFrameTime )
        .def_readonly( "triangleCount", &RenderWindow::FrameStats::triangleCount )
        .def_readonly( "batchCount", &RenderWindow::FrameStats::batchCount )
    ];
}

// Keep this at the bottom so we don't need prototypes for other bind functions.
void bindEngine( lua_State* L )
{
    bindGui( L );
    bindQuaternion( L );
    bindVector3( L );
    bindVector2( L );
    bindColourValue( L );
    bindEntity( L );
    bindSceneNode( L );
    bindCamera( L );
    bindRadian( L );
    bindFrameStats( L );

    module(L)
    [
        namespace_("Ogre")
        [
            def("getRootSceneNode", &getRootSceneNode ),
            def("getSceneNode", &getSceneNode ),
            def("sceneNodeExists", sceneNodeExists ),
            def("getCamera", &getCamera ),
            def("createEntity", &createEntity ),
            def("destroyEntity", &destroyEntity ),
            def("getStats", &getFrameStats )
        ]
    ];
}
