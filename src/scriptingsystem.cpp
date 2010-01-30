#include <scriptingsystem.h>
#include <OgreLogManager.h>
#include <luabind/class_info.hpp>
#include <inputeventdata.h>
#include "luaresource.h"

int ScriptingSystem::GUID = 0;

void queueEventThunk( lua_State *L, EventPtr event )
{
    using namespace luabind;

    lua_pushlightuserdata( L, (void *)&ScriptingSystem::GUID );
    lua_gettable( L, LUA_REGISTRYINDEX );

    ScriptingSystem* scriptingSystem = static_cast<ScriptingSystem*>(lua_touserdata( L, -1 ));

    if( scriptingSystem )
    {
        scriptingSystem->queueEvent( event );
    }
    else
    {
        Ogre::LogManager::getSingleton().stream() << "Error queuing event from Lua; could not retrieve scripting system instance";
    }
}

int luaLibraryLoader( lua_State *L )
{
    if( ! lua_isstring( L, 1 ) )
    {
        luaL_typerror( L, 1, "string" );
    }

    std::string libraryFile = lua_tostring( L, 1 );

    try
    {
        LuaResourcePtr library = LuaResourceManager::getSingleton().load( libraryFile + ".lua", "General" );

        luaL_loadstring( L, library->getScriptSource().c_str() );
    }
    catch( Ogre::FileNotFoundException& e )
    {
        // Could not find the file.
        std::string errMessage = "\n  no file '" + libraryFile + ".lua' found in Ogre resource archives.";
        lua_pushstring( L, errMessage.c_str() );
    }
    return 1;
}

void installLibraryLoader( lua_State *L )
{
    // Insert the c++ func 'luaLibraryLoader' into package.loaders.
    // Inserted at the start of the table in order to take precedance.
    lua_getglobal( L, "table" );
    lua_getfield( L, -1, "insert" );
    lua_remove( L, -2 );    // table
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "loaders" );
    lua_remove( L, -2 );    // package
    lua_pushnumber( L, 1 ); // index where to insert into loaders table
    lua_pushcfunction( L, luaLibraryLoader );
    if( lua_pcall( L, 3, 0, 0 ) )
        Ogre::LogManager::getSingleton().stream() << lua_tostring( L, 1);
}

void ScriptingSystem::shutdown()
{
    if( mL )
        lua_close( mL );
    mL = 0;
}

void ScriptingSystem::initialise()
{
    mL = lua_open();

    luaL_openlibs( mL );
    luabind::open( mL );

    lua_pushlightuserdata( mL, (void *)&GUID );
    lua_pushlightuserdata( mL, this );
    lua_settable( mL, LUA_REGISTRYINDEX );

    bind();

    LuaResourcePtr mainlua = LuaResourceManager::getSingleton().load( "main.lua" );

    if( luaL_dostring( mL, mainlua->getScriptSource().c_str() ) );
    {
        Ogre::LogManager::getSingleton().stream() << lua_tostring( mL, -1 );
    }
}

void ScriptingSystem::bind()
{
    using namespace luabind;

    bind_class_info(mL);
    installLibraryLoader( mL );

    module(mL)
    [
        class_<Event, EventPtr>( "Event" )
            .def( constructor<>() )
            .def( constructor<const char*>() )
            .def( constructor<std::size_t>() )
            .def_readwrite( "type", &Event::type )
            .def_readwrite( "data", &Event::data )
            .scope
            [
                def( "hash", &Event::hash )
            ]
        ,
        def( "queueEvent", &queueEventThunk )
        ,
        class_<EventData, boost::shared_ptr<EventData> >( "EventData" ),
        class_<InputEventData, EventData, boost::shared_ptr<EventData> >( "InputEventData" )
            .def( constructor<>() )
            .def_readwrite( "x", &InputEventData::x )
            .def_readwrite( "y", &InputEventData::y )
            .def_readwrite( "buttons", &InputEventData::parm )
            .def_readwrite( "text", &InputEventData::parm )
            .def_readwrite( "key", &InputEventData::key )
    ];
}

bool ScriptingSystem::EventNotification( EventPtr event )
{
    luabind::object notify = luabind::globals( mL )["EventNotification"];
    luabind::object ret;

    if( notify )
    {
        ret = notify( event );
        if( luabind::object_cast<bool>(ret) == true )
            return true;
    }

    return false;
}

bool ScriptingSystem::frameStarted(const Ogre::FrameEvent& evt)
{
    luabind::object func = luabind::globals( mL )["FrameStarted"];

    if( func )
        func( evt.timeSinceLastFrame );

    return true;
}

bool ScriptingSystem::frameEnded(const Ogre::FrameEvent& evt)
{
    luabind::object func = luabind::globals( mL )["FrameEnded"];

    if( func )
        func( evt.timeSinceLastFrame );

    return true;
}
