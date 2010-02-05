#include <Ogre.h>
#include <luabind/luabind.hpp>

// Prototype this before operator.hpp so it can be found for tostring() operator.
std::ostream& operator<<( std::ostream& stream, const Ogre::Entity ent );

#include <luabind/operator.hpp>

using namespace luabind;
using namespace Ogre;

// Some helpful macros for defining constants (sort of) in Lua.  Similar to this code:
// object g = globals(L);
// object table = g["class"];
// table["constant"] = class::constant;

#define LUA_CONST_START( class ) { object g = globals(L); object table = g[#class];
#define LUA_CONST( class, name ) table[#name] = class::name
#define LUA_CONST_END }

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
    LogManager::getSingleton().stream() << "Destroying overlay: " << overlay->getName();
    OverlayManager::getSingleton().destroy( overlay );
}

void DestroyOverlayElement( OverlayElement* element )
{
    LogManager::getSingleton().stream() << "Destroying overlay element: " << element->getName();
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

		// Operators

		.def( self + other<Vector3>() )
		.def( self - other<Vector3>() )
		.def( self * other<Vector3>() )
		.def( self * Real() )
	];

	LUA_CONST_START( Vector3 )
		LUA_CONST( Vector3, ZERO);
		LUA_CONST( Vector3, UNIT_X);
		LUA_CONST( Vector3, UNIT_Y);
		LUA_CONST( Vector3, UNIT_Z);
		LUA_CONST( Vector3, NEGATIVE_UNIT_X);
		LUA_CONST( Vector3, NEGATIVE_UNIT_Y);
		LUA_CONST( Vector3, NEGATIVE_UNIT_Z);
		LUA_CONST( Vector3, UNIT_SCALE);
	LUA_CONST_END;
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

	LUA_CONST_START( Vector2 )
		LUA_CONST( Vector2, ZERO);
		LUA_CONST( Vector2, UNIT_X);
		LUA_CONST( Vector2, UNIT_Y);
		LUA_CONST( Vector2, NEGATIVE_UNIT_X);
		LUA_CONST( Vector2, NEGATIVE_UNIT_Y);
		LUA_CONST( Vector2, UNIT_SCALE);
	LUA_CONST_END;
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
	];

	LUA_CONST_START( ColourValue )
		LUA_CONST( ColourValue, ZERO);
		LUA_CONST( ColourValue, Black);
		LUA_CONST( ColourValue, White);
		LUA_CONST( ColourValue, Red);
		LUA_CONST( ColourValue, Green);
		LUA_CONST( ColourValue, Blue);
	LUA_CONST_END;
}


std::ostream& operator<<( std::ostream& stream, const Entity ent )
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

// Fake member function for simplifing binding, as the real functions
// have optional aguments, which I don't want to use in the Lua script.
// However luabind does not support optional arguments.
// Think of "obj" as "this"
SceneNode *createChildSceneNode( SceneNode *obj, const String name )
{
	return obj->createChildSceneNode( name );
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
	];
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
	];
}

// Keep this at the bottom so we don't need prototypes for other bind functions.
void bindEngine( lua_State* L )
{
	bindGui( L );
	bindVector3( L );
	bindVector2( L );
	bindColourValue( L );
	bindEntity( L );
	bindSceneNode( L );
	bindCamera( L );

	module(L)
	[
        namespace_("Ogre")
        [
            def("getRootSceneNode", &getRootSceneNode ),
            def("getCamera", &getCamera ),
            def("createEntity", &createEntity )
        ]
    ];
}
