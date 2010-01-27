#include <scriptingsystem.h>
#include <OgreLogManager.h>
#include <luabind/class_info.hpp>

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

    if( luaL_dofile( mL, "main.lua" ) );
    {
        Ogre::LogManager::getSingleton().stream() << lua_tostring( mL, -1 );
    }
}

void ScriptingSystem::bind()
{
    using namespace luabind;

    bind_class_info(mL);

    module(mL)
    [
        class_<Event, EventPtr>( "Event" )
            .def( constructor<>() )
            .def( constructor<const char*>() )
            .def( constructor<std::size_t>() )
            .def_readwrite( "type", &Event::type )
            .def_readwrite( "key", &Event::key )
            .def_readwrite( "x", &Event::x )
            .def_readwrite( "y", &Event::y )
            .def_readwrite( "param1", &Event::param1 )
            .def_readwrite( "data", &Event::data )
            .scope
            [
                def( "hash", &Event::hash )
            ]
        ,
        def( "queueEvent", &queueEventThunk )
        ,
        class_<EventData, boost::shared_ptr<EventData> >( "EventData" )
            .def( constructor<>() ),
        class_<InputEventData, EventData, boost::shared_ptr<EventData> >( "InputEventData" )
            .def( constructor<>() )
            .def_readwrite( "x", &InputEventData::x )
            .def_readwrite( "y", &InputEventData::y )
            .def_readwrite( "button", &InputEventData::parm )
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
