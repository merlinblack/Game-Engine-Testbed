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
std::ostream& operator<<( std::ostream& stream, const Ogre::SceneManager& mgr );

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
        .def( self / Real() )
        .def( self == other<Vector3>() )
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
        .def( self / Real() )
        .def( self == other<Vector2>() )
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

void ManualObject_finish( ManualObject* self )
{
    self->end();
}

void bindEntity( lua_State* L ) // And Movable Object for now.
{
    module(L)
    [
        class_<MovableObject>("MovableObject")
        .def("getVisible", &MovableObject::getVisible )
        .def("setVisible", &MovableObject::setVisible )
        .def("detachFromParent", &MovableObject::detachFromParent ),
        class_<Entity, MovableObject>("Entity")
        .def(tostring(self))
        .def("setMaterialName", &Entity::setMaterialName )
        .def("setDisplaySkeleton", &Entity::setDisplaySkeleton ),
        class_<ManualObject, MovableObject>("ManualObject")
        .def(constructor<const String&>())
        .def("clear", &ManualObject::clear )
        .def("begin", &ManualObject::begin )
        .def("position", (void (ManualObject::*)(const Vector3&))&ManualObject::position )
        .def("position", (void (ManualObject::*)(Real, Real, Real))&ManualObject::position )
        .def("normal", (void (ManualObject::*)(const Vector3&))&ManualObject::normal )
        .def("normal", (void (ManualObject::*)(Real, Real, Real))&ManualObject::normal )
        .def("tangent", (void (ManualObject::*)(const Vector3&))&ManualObject::tangent )
        .def("tangent", (void (ManualObject::*)(Real, Real, Real))&ManualObject::tangent )
        .def("texureUV", (void (ManualObject::*)(Real, Real))&ManualObject::textureCoord )
        .def("texureUV", (void (ManualObject::*)(const Vector2&))&ManualObject::textureCoord )
        .def("colour", (void (ManualObject::*)(Real, Real, Real, Real))&ManualObject::colour )
        .def("colour", (void (ManualObject::*)(const ColourValue&))&ManualObject::colour )
        .def("index",  &ManualObject::index )
        .def("finish", &ManualObject_finish )
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
        .def("lookAt", &SceneNode::lookAt )
        .def("setAutoTracking", &SceneNode::setAutoTracking )
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

void bindLight( lua_State* L )
{
    module(L)
    [
        class_<Light, MovableObject>("Light")
        .property( "type", &Light::getType, &Light::setType )
        .def( "setDiffuse", (void( Light::*)(Real, Real, Real))&Light::setDiffuseColour )
        .def( "setDiffuse", (void( Light::*)(const ColourValue&))&Light::setDiffuseColour )
        .def( "setSpecular", (void( Light::*)(Real, Real, Real))&Light::setSpecularColour )
        .def( "setSpecular", (void( Light::*)(const ColourValue&))&Light::setSpecularColour )
        .def( "setAttenuation", &Light::setAttenuation )
        .def( "setPosition", (void( Light::*)(Real, Real, Real))&Light::setPosition )
        .def( "setPosition", (void( Light::*)(const Vector3&))&Light::setPosition )
        .def( "setDirection", (void( Light::*)(Real, Real, Real))&Light::setDirection )
        .def( "setDirection", (void( Light::*)(const Vector3&))&Light::setDirection )
    ];

    LUA_STATIC_START( Light )
        LUA_STATIC( Light, LT_POINT );
        LUA_STATIC( Light, LT_DIRECTIONAL );
        LUA_STATIC( Light, LT_SPOTLIGHT );
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

void Scene_setSkyBox( SceneManager* mgr, String materialName )
{
    mgr->setSkyBox( true, materialName );
}

void Scene_disableSkyBox( SceneManager* mgr )
{
    mgr->setSkyBox( false, String() );
}

void bindSceneManager( lua_State* L )
{
    module(L)
    [
        class_<SceneManager>("SceneManager")
        .def("getCamera", &SceneManager::getCamera )
        .def("getRootSceneNode", &SceneManager::getRootSceneNode )
        .def("getSceneNode", &SceneManager::getSceneNode )
        .def("hasSceneNode", &SceneManager::hasSceneNode )
        .def("destroySceneNode", (void (SceneManager::*)(const String&))&SceneManager::destroySceneNode )
        .def("destroySceneNode", (void (SceneManager::*)(SceneNode*))&SceneManager::destroySceneNode )
        .def("createEntity", (Entity* (SceneManager::*)(const String&, const String&, const String&))&SceneManager::createEntity )
        .def("createEntity", (Entity* (SceneManager::*)(const String&))&SceneManager::createEntity )
        .def("getEntity", &SceneManager::getEntity )
        .def("hasEntity", &SceneManager::hasEntity )
        .def("destroyEntity", (void (SceneManager::*)(Entity*))&SceneManager::destroyEntity )
        .def("destroyEntity", (void (SceneManager::*)(const String&))&SceneManager::destroyEntity )
        .def("destroyAllEntities", &SceneManager::destroyAllEntities)
        .def("clearScene", &SceneManager::clearScene )
        .def("setAmbientLight", &SceneManager::setAmbientLight )
        .def("getAmbientLight", &SceneManager::getAmbientLight )
        .def("setDisplaySceneNodes", &SceneManager::setDisplaySceneNodes )
        .def("setShadowTechnique", &SceneManager::setShadowTechnique )
        .def("setSkyBox", &Scene_setSkyBox )
        .def("disableSkyBox", &Scene_disableSkyBox )

        .def(tostring(self))
    ];

    LUA_STATIC_START( SceneManager )
        table["AUTODETECT_RESOURCE_GROUP_NAME"] = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;
        table["SHADOWTYPE_NONE"] = SHADOWTYPE_NONE;
        table["SHADOWTYPE_STENCIL_ADDITIVE"] = SHADOWTYPE_STENCIL_ADDITIVE;
        table["SHADOWTYPE_STENCIL_MODULATIVE"] = SHADOWTYPE_STENCIL_MODULATIVE;
        table["SHADOWTYPE_TEXTURE_ADDITIVE"] = SHADOWTYPE_TEXTURE_ADDITIVE;
        table["SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED"] = SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;
        table["SHADOWTYPE_TEXTURE_MODULATIVE"] = SHADOWTYPE_TEXTURE_MODULATIVE;
        table["SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED"] = SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
    LUA_STATIC_END;
}

std::ostream& operator<<( std::ostream& stream, const Ogre::SceneManager& mgr )
{
    return stream << "Scene Manager: " << mgr.getName();
}

SceneManager* getSceneManager()
{
    Root* root = Root::getSingletonPtr();
    return root->getSceneManager("SceneManagerInstance");
}

void bindRadian( lua_State* L )
{
    module(L)
    [
        class_<Radian>("Radian")
        .def(constructor< Real >() )
        .def(constructor< Degree& >() )
        .def(tostring(self))
        .def_readonly( "rad", (float (Radian::*)())&Radian::valueRadians )
        .def_readonly( "deg", (float (Radian::*)())&Radian::valueDegrees ),

        class_<Degree>("Degree")
        .def(constructor< Real >() )
        .def(constructor< Radian& >() )
        .def(tostring(self))
        .def_readonly( "rad", (float (Degree::*)())&Degree::valueRadians )
        .def_readonly( "deg", (float (Degree::*)())&Degree::valueDegrees )
    ];
}

RenderWindow::FrameStats getFrameStats()
{
    Root* root = Root::getSingletonPtr();
    RenderWindow* window = root->getAutoCreatedWindow();

    return window->getStatistics();
}

unsigned short getNumViewports()
{
    RenderWindow* window = Root::getSingletonPtr()->getAutoCreatedWindow();

    return window->getNumViewports();
}

Viewport* getViewport( unsigned short index )
{
    RenderWindow* window = Root::getSingletonPtr()->getAutoCreatedWindow();

    return window->getViewport( index );
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

String ViewportToString( Viewport* )
{
    return "Viewport";
}

void bindViewport( lua_State *L )
{
    module(L)
    [
        class_<Viewport>("Viewport")
        .def("__tostring", &ViewportToString)
    ];
}

// Keep this at the bottom so we don't need prototypes for other bind functions.
void bindOgre( lua_State* L )
{
    bindQuaternion( L );
    bindVector3( L );
    bindVector2( L );
    bindColourValue( L );
    bindEntity( L );
    bindSceneNode( L );
    bindCamera( L );
    bindLight( L );
    bindSceneManager( L );
    bindRadian( L );
    bindFrameStats( L );
    bindViewport( L );

    module(L)
    [
        namespace_("Ogre")
        [
            def("getStats", &getFrameStats ),
            def("getSceneManager", &getSceneManager ),
            def("getNumViewports", &getNumViewports ),
            def("getViewport", &getViewport )
        ]
    ];
}
