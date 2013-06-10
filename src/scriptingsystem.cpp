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
#include <scriptingsystem.h>
#include <OgreLogManager.h>
#include <inputeventdata.h>
#include <windoweventdata.h>
#include <animationeventdata.h>
#include <luaresource.h>
#include <gameentity.h>
#include <version.h>
#include <LuaBridge.h>

/*
void bindOgre( lua_State* L ); // From ogrebind.cpp
void bindAnimation( lua_State* L ); // From animationbind.cpp
void bindNavigationMesh( lua_State* L); // From navmeshbind.cpp
*/
void bindGorilla( lua_State *L ); // From gorillabind.cpp

template<class D, class B>
D* downcast( B* ptr )
{
    return dynamic_cast<D*>( ptr );
}

int ScriptingSystem::GUID = 0;

void queueEventThunk( Event& event, lua_State *L )
{
    lua_pushlightuserdata( L, (void *)&ScriptingSystem::GUID );
    lua_gettable( L, LUA_REGISTRYINDEX );

    ScriptingSystem* scriptingSystem = static_cast<ScriptingSystem*>(lua_touserdata( L, -1 ));

    if( scriptingSystem )
    {
        scriptingSystem->queueEvent( EventPtr( new Event(event) ) );
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
        luaL_error( L, "luaLibraryLoader: Expected string for first parameter" );
    }

    std::string libraryFile = lua_tostring( L, 1 );
    // In order to be compatible with the normal Lua file loader,
    // translate '.' to the file system seperator character.
    // In this case (An ogre resource) '/'
    while( libraryFile.find('.') != std::string::npos )
        libraryFile.replace( libraryFile.find('.'), 1, "/" );

    libraryFile += ".lua";

    if( ! Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup( libraryFile ) )
    {
        // Could not find the file.
        std::string errMessage = "\n  no file '" + libraryFile + "' found in Ogre resource archives.";
        lua_pushstring( L, errMessage.c_str() );
    }
    else
    {
        LuaResourcePtr library = LuaResourceManager::getSingleton().load( libraryFile, "General" );

        if( luaL_loadbuffer( L, library->getScriptSource().c_str(), library->calculateSize(), libraryFile.c_str() ) )
        {
            luaL_error( L, "Error loading library '%s' from resource archive.\n%s", libraryFile.c_str(), lua_tostring( L, -1 ) );
        }
    }
    return 1;
}

void installLibraryLoader( lua_State *L )
{
    // Insert the c++ func 'luaLibraryLoader' into package.loaders.
    // Inserted at the start of the table in order to take precedence.
    lua_getglobal( L, "table" );
    lua_getfield( L, -1, "insert" );
    lua_remove( L, -2 );    // table
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "searchers" );
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
    mL = luaL_newstate();

    luaL_openlibs( mL );

    lua_pushlightuserdata( mL, (void *)&GUID );
    lua_pushlightuserdata( mL, this );
    lua_settable( mL, LUA_REGISTRYINDEX );

    bind();
    /*
    bindOgre( mL );
    bindGameEntityClasses( mL );
    bindAnimation( mL );
    bindNavigationMesh( mL );
    */
    bindGorilla( mL );

    using namespace luabridge;

    LuaResourcePtr mainlua = LuaResourceManager::getSingleton().load( "main.lua" );

    if( luaL_loadbuffer( mL,
                mainlua->getScriptSource().c_str(),
                mainlua->calculateSize(), "main.lua" ) || lua_pcall( mL, 0, LUA_MULTRET, 0) )
    {
        Ogre::LogManager::getSingleton().stream() << " ****************************** ";
        Ogre::LogManager::getSingleton().stream() << " *** main.lua failed to run     ";
        Ogre::LogManager::getSingleton().stream() << " *** " << lua_tostring( mL, -1 );
        Ogre::LogManager::getSingleton().stream() << " ****************************** ";
    }
}

void ScriptingSystem::bind()
{
    using namespace luabridge;

    installLibraryLoader( mL );

    getGlobalNamespace( mL )
        .beginNamespace( "Engine" )
        .beginClass<Event>( "Event" )
            .addConstructor<void (*) (const char*)>()
            .addData( "type", &Event::type )
            .addData( "data", &Event::data )
            .addStaticFunction( "hash", &Event::hash )
        .endClass()
        .addFunction( "queueEvent", &queueEventThunk )
        .beginClass<EventData>( "EventData" )
        .endClass()
        .deriveClass<InputEventData, EventData>( "InputEventData" )
            .addConstructor<void (*) (void)>()
            .addStaticFunction( "downcast", &downcast<InputEventData, EventData> )
            .addData( "x", &InputEventData::x )
            .addData( "y", &InputEventData::y )
            .addData( "buttons", &InputEventData::parm )
            .addProperty( "text", &InputEventDataBindingHelper::getText, &InputEventDataBindingHelper::setText )
            .addProperty( "key",  &InputEventDataBindingHelper::getKey,  &InputEventDataBindingHelper::setKey  )
        .endClass()
        .deriveClass<WindowEventData, EventData>( "WindowEventData" )
            .addStaticFunction( "downcast", &downcast<WindowEventData, EventData> )
            .addData( "width", &WindowEventData::width )
            .addData( "height", &WindowEventData::height )
        .endClass()
        /*
        .deriveClass<AnimationEventData, EventData>( "AnimationEventData" )
            .addData( "animation", &AnimationEventData::animation )
        .endClass()
        */
        .addFunction( "versionString", versionString )
    .endNamespace();
}

bool ScriptingSystem::EventNotification( EventPtr event )
{
    luabridge::LuaRef notify = luabridge::getGlobal( mL, "EventNotification");
    luabridge::LuaRef ret( mL );

    if( notify )
    {
        try
        {
            ret = notify( *event );
            if( ret == true )
                return true;
        }
        catch( luabridge::LuaException& e )
        {
            Ogre::LogManager::getSingleton().stream() << e.what();
        }
    }

    return false;
}

bool ScriptingSystem::frameStarted(const Ogre::FrameEvent& evt)
{
    luabridge::LuaRef func = luabridge::getGlobal( mL, "FrameStarted" );

    try
    {
        // If the function is not defined, don't worry about it.
        if( func )
            func( evt.timeSinceLastFrame );
    }
    catch( luabridge::LuaException& e )
    {
        Ogre::LogManager::getSingleton().stream() << e.what();
    }

    return true;
}

bool ScriptingSystem::frameEnded(const Ogre::FrameEvent& evt)
{
    luabridge::LuaRef func = luabridge::getGlobal( mL, "FrameEnded" );

    try
    {
        // If the function is not defined, don't worry about it.
        if( func )
            func( evt.timeSinceLastFrame );
    }
    catch( luabridge::LuaException& e )
    {
        Ogre::LogManager::getSingleton().stream() << e.what();
    }

    // Do a Lua garbage collection.
    lua_gc( mL, LUA_GCSTEP, 1 );

    return true;
}
