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
#include <lua.hpp>
#include <LuaBridge.h>
#include <luareadonlytable.h>

using namespace luabridge;
using namespace Ogre;

void bindQuaternion( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<Quaternion>( "Quaternion" )
        .addData( "x", &Quaternion::x )
        .addData( "y", &Quaternion::y )
        .addData( "z", &Quaternion::z )
        .addData( "w", &Quaternion::w )
        .addFunction( "dot", &Quaternion::Dot )
        .addConstructor<void (*) (Real,Real,Real,Real)>()
        .addFunction( "__add", (Quaternion (Quaternion::*)(const Quaternion&) const)&Quaternion::operator+ )
        .addFunction( "__sub", (Quaternion (Quaternion::*)(const Quaternion&) const)&Quaternion::operator- )
        .addFunction( "__mul", (Quaternion (Quaternion::*)(const Quaternion&) const)&Quaternion::operator* )
        // May have to write an overide handler for __mul. See luabind operator binding below...
        .endClass()
        .endNamespace();

    LuaRef ogre = getGlobal( L, "Ogre" );
    LuaRef quaternion = ogre["Quaternion"];
    quaternion["ZERO"]     = Quaternion::ZERO;
    quaternion["IDENTITY"] = Quaternion::IDENTITY;

    /*
        // Operators
        .def( self + other<Quaternion>() )
        .def( self - other<Quaternion>() )
        .def( self * other<Quaternion>() )
        .def( self * Real() )
        .def( self * other<Vector3>() )
    */
}

Vector3* Vector3Constructor( lua_State* L )
{
    const char* arg_error = "Expecting %s for argument %d when given %d arguments";

    switch( lua_gettop( L ) ) // Note that the class table is always index 1 on the stack.
    {
        case 1:
            {
                return new Vector3();
            }
            break;
        case 2:
            {
                LuaRef p1 = LuaRef::fromStack( L, 2 );
                if( p1.is<Vector3>() == true )
                {
                    return new Vector3( LuaRef_cast<Vector3>(p1) );
                }
                luaL_error( L, arg_error, "Vector3", 1, 1 ); // returns
            }
            break;
        case 4:
            {
                LuaRef p1 = LuaRef::fromStack( L, 2 );
                LuaRef p2 = LuaRef::fromStack( L, 3 );
                LuaRef p3 = LuaRef::fromStack( L, 4 );

                if( p1.isNumber() != true )
                    luaL_error( L, arg_error, "number", 1, 3 ); // returns
                if( p2.isNumber() != true )
                    luaL_error( L, arg_error, "number", 2, 3 ); // returns
                if( p2.isNumber() != true )
                    luaL_error( L, arg_error, "number", 3, 3 ); // returns

                return new Vector3(
                        LuaRef_cast<Real>( p1 ),
                        LuaRef_cast<Real>( p2 ),
                        LuaRef_cast<Real>( p3 ) );
            }
            break;
        default:
            luaL_error( L, "Incorrect number of arguments given (%d)", lua_gettop( L ) ); // returns
            break;
    }

    // Should not get here.
    return NULL;
}

String Vector3ToString( const Vector3* ptr )
{
    std::stringstream ss;

    ss << "Vector3(" << ptr->x << ", " << ptr->y << ", " << ptr->z << ")";
    return ss.str();
}

void bindVector3( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<Vector3>( "Vector3" )
        .addData( "x", &Vector3::x )
        .addData( "y", &Vector3::y )
        .addData( "z", &Vector3::z )
        .addStaticFunction( "__call", &Vector3Constructor )
        .addFunction( "__add", (Vector3  (Vector3::*) (const Vector3&) const )&Vector3::operator+ )
        .addFunction( "__sub", (Vector3  (Vector3::*) (const Vector3&) const )&Vector3::operator- )
        .addFunction( "__mul", (Vector3  (Vector3::*) (const Vector3&) const )&Vector3::operator* )
        .addFunction( "__div", (Vector3  (Vector3::*) (const Real)     const )&Vector3::operator/ )
        .addFunction( "__eq",  (Vector3& (Vector3::*) (const Vector3&)       )&Vector3::operator= )
        .addFunction( "__tostring", &Vector3ToString )
        .addFunction("absDotProduct", &Vector3::absDotProduct)
        .addFunction("crossProduct", &Vector3::crossProduct )
        .addFunction("directionEquals", &Vector3::directionEquals )
        .addFunction("distance", &Vector3::distance )
        .addFunction("dotProduct", &Vector3::dotProduct )
        .addFunction("getRotationTo", &Vector3::getRotationTo )
        .addFunction("isZeroLength", &Vector3::isZeroLength )
        .addFunction("length", &Vector3::length )
        .addFunction("makeCeil", &Vector3::makeCeil )
        .addFunction("makeFloor", &Vector3::makeFloor )
        .addFunction("midPoint", &Vector3::midPoint )
        .addFunction("normalise", &Vector3::normalise )
        .addFunction("normalisedCopy", &Vector3::normalisedCopy )
        .addFunction("perpendicular", &Vector3::perpendicular )
        .addFunction("positionCloses", &Vector3::positionCloses )
        .addFunction("positionEquals", &Vector3::positionEquals )
        .addFunction("randomDeviant", &Vector3::randomDeviant )
        .addFunction("reflect", &Vector3::reflect )
        .addFunction("squaredDistance", &Vector3::squaredDistance )
        .addFunction("squaredLength", &Vector3::squaredLength )
        .addFunction("angleBetween", &Vector3::angleBetween )
        .endClass()
        .endNamespace();

    LuaRef ogre = getGlobal( L, "Ogre" );
    LuaRef vector3 = ogre["Vector3"];

    vector3["ZERO"] = Vector3::ZERO;
    vector3["UNIT_X"] = Vector3::UNIT_X;
    vector3["UNIT_Y"] = Vector3::UNIT_Y;
    vector3["UNIT_Z"] = Vector3::UNIT_Z;
    vector3["NEGATIVE_UNIT_X"] = Vector3::NEGATIVE_UNIT_X;
    vector3["NEGATIVE_UNIT_Y"] = Vector3::NEGATIVE_UNIT_Y;
    vector3["NEGATIVE_UNIT_Z"] = Vector3::NEGATIVE_UNIT_Z;
    vector3["UNIT_SCALE"] = Vector3::UNIT_SCALE;
}

void bindVector2( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<Vector2>( "Vector2" )
        .addData( "x", &Vector2::x )
        .addData( "y", &Vector2::y )
        .addConstructor<void (*) (Real,Real)>()
        .addFunction( "__add", (Vector2  (Vector2::*) (const Vector2&) const )&Vector2::operator+ )
        .addFunction( "__sub", (Vector2  (Vector2::*) (const Vector2&) const )&Vector2::operator- )
        .addFunction( "__mul", (Vector2  (Vector2::*) (const Vector2&) const )&Vector2::operator* )
        .addFunction( "__div", (Vector2  (Vector2::*) (const Real)     const )&Vector2::operator/ )
        .addFunction( "__eq",  (Vector2& (Vector2::*) (const Vector2&)       )&Vector2::operator= )
        .addFunction("crossProduct", &Vector2::crossProduct )
        .addFunction("dotProduct", &Vector2::dotProduct )
        .addFunction("isZeroLength", &Vector2::isZeroLength )
        .addFunction("length", &Vector2::length )
        .addFunction("makeCeil", &Vector2::makeCeil )
        .addFunction("makeFloor", &Vector2::makeFloor )
        .addFunction("midPoint", &Vector2::midPoint )
        .addFunction("normalise", &Vector2::normalise )
        .addFunction("normalisedCopy", &Vector2::normalisedCopy )
        .addFunction("perpendicular", &Vector2::perpendicular )
        .addFunction("randomDeviant", &Vector2::randomDeviant )
        .addFunction("reflect", &Vector2::reflect )
        .addFunction("squaredLength", &Vector2::squaredLength )
        .endClass()
        .endNamespace();

    LuaRef ogre = getGlobal( L, "Ogre" );
    LuaRef vector2 = ogre["Vector2"];

    vector2["ZERO"] = Vector2::ZERO;
    vector2["UNIT_X"] = Vector2::UNIT_X;
    vector2["UNIT_Y"] = Vector2::UNIT_Y;
    vector2["NEGATIVE_UNIT_X"] = Vector2::NEGATIVE_UNIT_X;
    vector2["NEGATIVE_UNIT_Y"] = Vector2::NEGATIVE_UNIT_Y;
    vector2["UNIT_SCALE"] = Vector2::UNIT_SCALE;
}

void bindColourValue( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<ColourValue>( "ColourValue" )
        .addConstructor<void (*) (Real,Real,Real,Real)>()
        .addData( "r", &ColourValue::r )
        .addData( "g", &ColourValue::g )
        .addData( "b", &ColourValue::b )
        .addData( "a", &ColourValue::a )
        .addFunction( "saturate", &ColourValue::saturate )
        .endClass()
        .endNamespace();
    // Not bothering with operators - do them when needed.

    LuaRef ogre = getGlobal( L, "Ogre" );
    LuaRef colourvalue = ogre["ColourValue"];

    colourvalue["ZERO"]  = ColourValue::ZERO;
    colourvalue["Black"] = ColourValue::Black;
    colourvalue["White"] = ColourValue::White;
    colourvalue["Red"]   = ColourValue::Red;
    colourvalue["Green"] = ColourValue::Green;
    colourvalue["Blue"]  = ColourValue::Blue;
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
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<MovableObject>("MovableObject")
        .addFunction( "getVisible", &MovableObject::getVisible )
        .addFunction( "setVisible", &MovableObject::setVisible )
        .endClass()
        .deriveClass<Entity,MovableObject>("Entity")
        .addFunction("setMaterialName", &Entity::setMaterialName )
        .addFunction("setDisplaySkeleton", &Entity::setDisplaySkeleton )
        .endClass()
        .endNamespace();

    /*
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
    */
}

// Fake member function for simplifying binding, as the real functions
// have optional arguments, which I don't want to use in the Lua script.
// However luabridge does not support optional arguments.
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

void SceneNode_translate( SceneNode *obj, const Vector3& v, int transformspace )
{
    return obj->translate( v, (SceneNode::TransformSpace) transformspace );
}

Vector3 SceneNode_getPosition( SceneNode* obj )
{
    return obj->getPosition();
}

Quaternion SceneNode_getOrientation( SceneNode* obj )
{
    return obj->getOrientation();
}

std::ostream& operator<<( std::ostream& stream, const SceneNode& node )
{
    return stream << node.getName();
}

void bindSceneNode( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<SceneNode>("SceneNode")
        .addFunction( "createChildSceneNode", &createChildSceneNode )
        .addFunction( "attachObject", &SceneNode::attachObject )
        .addFunction( "yaw", SceneNode_yaw )
        .addFunction( "pitch", SceneNode_pitch )
        .addFunction( "roll", SceneNode_roll )
        .addFunction( "setPosition", (void( SceneNode::*)(Real,Real,Real))&SceneNode::setPosition )
        .addFunction( "setPositionV3", (void( SceneNode::*)(const Vector3&))&SceneNode::setPosition )
        .addFunction( "getPosition", &SceneNode_getPosition )
        .addFunction( "setOrientation", (void( SceneNode::*)( const Quaternion& ))&SceneNode::setOrientation )
        .addFunction( "getOrientation", &SceneNode_getOrientation )
        .addFunction( "scale", (void( SceneNode::*)(Real,Real,Real))&SceneNode::scale )
        .addFunction( "scaleV3", (void( SceneNode::*)(const Vector3&))&SceneNode::scale )
        .addFunction( "showBoundingBox", &SceneNode::showBoundingBox )
        .addFunction( "rotate", &SceneNode_rotate )
        .addFunction( "lookAt", &SceneNode::lookAt )
        .addFunction( "setAutoTracking", &SceneNode::setAutoTracking )
        .addFunction( "translate", &SceneNode_translate )
        .addFunction( "destroy", &SceneNode_destroy )
        .endClass()
        .endNamespace();

    LuaRef ogre = getGlobal( L, "Ogre" );
    LuaRef scenenode = ogre["SceneNode"];
    scenenode["TS_LOCAL"]  = (int)SceneNode::TS_LOCAL;
    scenenode["TS_PARENT"] = (int)SceneNode::TS_PARENT;
    scenenode["TS_WORLD"]  = (int)SceneNode::TS_WORLD;
}

void bindLight( lua_State* L )
{
    /*
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
    */
}

void bindCamera( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .deriveClass<Frustum,MovableObject>("Frustum")
        .endClass()
        .deriveClass<Camera,Frustum>("Camera")
        .addFunction("setPositionV3", (void( Camera::*)(const Vector3&))&Camera::setPosition )
        .addFunction("setPosition", (void( Camera::*)(Real,Real,Real))&Camera::setPosition )
        .addFunction("lookAtV3", (void( Camera::*)(const Vector3&))&Camera::lookAt )
        .addFunction("lookAt", (void( Camera::*)(Real,Real,Real))&Camera::lookAt )
        .addFunction("setNearClipDistance", &Camera::setNearClipDistance )
        .addFunction("setFarClipDistance", &Camera::setFarClipDistance )
        .endClass()
        .endNamespace();
}

void Scene_setSkyBox( SceneManager* mgr, const String& materialName )
{
    mgr->setSkyBox( true, materialName );
}

void Scene_disableSkyBox( SceneManager* mgr )
{
    mgr->setSkyBox( false, String() );
}

void Scene_setShadowTechnique( SceneManager *mgr, int technique )
{
    mgr->setShadowTechnique( (ShadowTechnique)technique );
}

void bindSceneManager( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<SceneManager>("SceneManager")
        .addFunction("getCamera", &SceneManager::getCamera )
        .addFunction("getRootSceneNode", &SceneManager::getRootSceneNode )
        .addFunction("getSceneNode", &SceneManager::getSceneNode )
        .addFunction("hasSceneNode", &SceneManager::hasSceneNode )
        .addFunction("destroySceneNodeByName", (void (SceneManager::*)(const String&))&SceneManager::destroySceneNode )
        .addFunction("destroySceneNode", (void (SceneManager::*)(SceneNode*))&SceneManager::destroySceneNode )
        .addFunction("createEntityEx", (Entity* (SceneManager::*)(const String&, const String&, const String&))&SceneManager::createEntity )
        .addFunction("createEntity", (Entity* (SceneManager::*)(const String&))&SceneManager::createEntity )
        .addFunction("getEntity", &SceneManager::getEntity )
        .addFunction("hasEntity", &SceneManager::hasEntity )
        .addFunction("destroyEntity", (void (SceneManager::*)(Entity*))&SceneManager::destroyEntity )
        .addFunction("destroyEntityByName", (void (SceneManager::*)(const String&))&SceneManager::destroyEntity )
        .addFunction("destroyAllEntities", &SceneManager::destroyAllEntities)
        .addFunction("clearScene", &SceneManager::clearScene )
        .addFunction("setAmbientLight", &SceneManager::setAmbientLight )
        .addFunction("getAmbientLight", &SceneManager::getAmbientLight )
        .addFunction("setDisplaySceneNodes", &SceneManager::setDisplaySceneNodes )
        .addFunction("setShadowTechnique", &Scene_setShadowTechnique )
        .addFunction("setSkyBox", &Scene_setSkyBox )
        .addFunction("disableSkyBox", &Scene_disableSkyBox )
        .addFunction("createLight", (Light* (SceneManager::*)())&SceneManager::createLight )
        .addFunction("createLightByName", (Light* (SceneManager::*)(const String&))&SceneManager::createLight )
        .endClass()
        .endNamespace();

    LuaRef ogre = getGlobal( L, "Ogre" );
    LuaRef sm = ogre["SceneManager"];
    sm["AUTODETECT_RESOURCE_GROUP_NAME"] = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;
    sm["SHADOWTYPE_NONE"] = (int) SHADOWTYPE_NONE;
    sm["SHADOWTYPE_STENCIL_ADDITIVE"] = (int) SHADOWTYPE_STENCIL_ADDITIVE;
    sm["SHADOWTYPE_STENCIL_MODULATIVE"] = (int) SHADOWTYPE_STENCIL_MODULATIVE;
    sm["SHADOWTYPE_TEXTURE_ADDITIVE"] = (int) SHADOWTYPE_TEXTURE_ADDITIVE;
    sm["SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED"] = (int) SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;
    sm["SHADOWTYPE_TEXTURE_MODULATIVE"] = (int) SHADOWTYPE_TEXTURE_MODULATIVE;
    sm["SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED"] = (int) SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
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

Radian* RadianConstructor( lua_State* L )
{
    const char* arg_error = "Expecting %s for argument %d when given %d arguments";

    switch( lua_gettop( L ) ) // Note that the class table is always index 1 on the stack.
    {
        case 1:
            {
                return new Radian();
            }
            break;
        case 2:
            {
                LuaRef p1 = LuaRef::fromStack( L, 2 );
                if( p1.isNumber() == true )
                {
                    return new Radian( LuaRef_cast<Real>(p1) );
                }
                if( p1.is<Degree>() == true )
                {
                    return new Radian( LuaRef_cast<Degree>(p1) );
                }
                luaL_error( L, arg_error, "Degree or Number", 1, 1 ); // returns
            }
            break;
        default:
            luaL_error( L, "Incorrect number of arguments given (%d)", lua_gettop( L ) ); // returns
            break;
    }

    // Should not get here.
    return NULL;
}

Degree* DegreeConstructor( lua_State* L )
{
    const char* arg_error = "Expecting %s for argument %d when given %d arguments";

    switch( lua_gettop( L ) ) // Note that the class table is always index 1 on the stack.
    {
        case 1:
            {
                return new Degree();
            }
            break;
        case 2:
            {
                LuaRef p1 = LuaRef::fromStack( L, 2 );
                if( p1.isNumber() == true )
                {
                    return new Degree( LuaRef_cast<Real>(p1) );
                }
                if( p1.is<Radian>() == true )
                {
                    return new Degree( LuaRef_cast<Radian>(p1) );
                }
                luaL_error( L, arg_error, "Radian or Number", 1, 1 ); // returns
            }
            break;
        default:
            luaL_error( L, "Incorrect number of arguments given (%d)", lua_gettop( L ) ); // returns
            break;
    }

    // Should not get here.
    return NULL;
}

void bindRadian( lua_State* L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<Radian>( "Radian" )
        .addStaticFunction( "__call", &RadianConstructor )
        .addFunction( "rad", (float (Radian::*)())&Radian::valueRadians )
        .addFunction( "deg", (float (Radian::*)())&Radian::valueDegrees )
        .endClass()
        .beginClass<Degree>( "Degree" )
        .addStaticFunction( "__call", &DegreeConstructor )
        .addFunction( "rad", (float (Degree::*)())&Degree::valueRadians )
        .addFunction( "deg", (float (Degree::*)())&Degree::valueDegrees )
        .endClass()
        .endNamespace();
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
    getGlobalNamespace( L )
        .beginNamespace("Ogre")
        .beginClass<RenderWindow::FrameStats>("FrameStats")
        .addData( "lastFPS", &RenderWindow::FrameStats::lastFPS )
        .addData( "avgFPS", &RenderWindow::FrameStats::avgFPS )
        .addData( "bestFPS", &RenderWindow::FrameStats::bestFPS )
        .addData( "worstFPS", &RenderWindow::FrameStats::worstFPS )
        .addData( "bestFrameTime", &RenderWindow::FrameStats::bestFrameTime )
        .addData( "worstFrameTime", &RenderWindow::FrameStats::worstFrameTime )
        .addData( "triangleCount", &RenderWindow::FrameStats::triangleCount )
        .addData( "batchCount", &RenderWindow::FrameStats::batchCount )
        .endClass()
        .endNamespace();
}

String ViewportToString( const Viewport* ptr )
{
    std::stringstream ss;

    ss << "Viewport - " << std::hex << ptr;
    return ss.str();
}

void bindViewport( lua_State *L )
{
    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .beginClass<Viewport>( "Viewport" )
        .addFunction( "__tostring", &ViewportToString )
        .endClass()
        .endNamespace();
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

    getGlobalNamespace( L )
        .beginNamespace( "Ogre" )
        .addFunction( "getStats", &getFrameStats )
        .addFunction( "getSceneManager", &getSceneManager )
        .addFunction( "getNumViewports", &getNumViewports )
        .addFunction( "getViewport", &getViewport )
        .endNamespace();
}
