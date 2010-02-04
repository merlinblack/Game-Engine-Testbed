#include <engine.h>
#include <inputeventdata.h>
#include <windoweventdata.h>
#include <OgreLogManager.h>
#include <luaresource.h>

void bindLuaConsole( lua_State *L );	// From luaconsolebinding.cpp

Engine::~Engine()
{
    // Order of shutdown matters.
    console             .shutdown();
    scriptingSystem     .shutdown();
    inputSystem         .shutdown();
    renderSystem        .shutdown();
}

bool Engine::initialise()
{
    // Managers may enque events as the start
    eventManager.addListener( this );
    eventManager.addListener( &renderSystem );
    eventManager.addListener( &scriptingSystem );
    eventManager.addListener( &inputSystem );

    if( ! renderSystem.initialise() )
        return false;

    inputSystem.initialise( renderSystem.getWindow(), false );
    scriptingSystem.initialise();

    // TODO: This could be better done by making the renderSystem a listener,
    // and send frame events.
    renderSystem.addFrameListener( &scriptingSystem );

    console.init( renderSystem.getRoot(), scriptingSystem.getInterpreter() );
	bindLuaConsole( scriptingSystem.getInterpreter() );

    Ogre::WindowEventUtilities::addWindowEventListener(renderSystem.getWindow(), this);

    stop = false;
}

void Engine::run()
{
    while( stop == false )
    {
        Ogre::WindowEventUtilities::messagePump();
        inputSystem.capture();
        eventManager.processEvents();
        renderSystem.renderOneFrame();
    }
}

bool Engine::EventNotification( EventPtr event )
{
    if( event->type == Event::hash( "MSG_QUIT" ) )
    {
        stop = true;
        return true;
    }

    if( event->type == Event::hash( "EVT_KEYDOWN" ) )
    {
        boost::shared_ptr<InputEventData> data = boost::dynamic_pointer_cast<InputEventData>( event->data );

        if( data->key == OIS::KC_F12 )   // Emergency stop.
        {
            stop = true;
            return true;
        }
        if( data->key == OIS::KC_GRAVE )
        {
            console.setVisible( ! console.isVisible() );
            return true;
        }
        if( console.isVisible() )
        {
            OIS::KeyEvent arg( 0, data->key, data->parm );
            console.injectKeyPress( arg );
            return true;
        }
    }

    return false;
}

//Adjust mouse clipping area
void Engine::windowResized( Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int top, left;

    EventPtr event( new Event( "EVT_WINDOW_RESIZE" ) );
    boost::shared_ptr<WindowEventData> data( new WindowEventData );

    rw->getMetrics(width, height, depth, left, top);

    event->data = data;
    data->height = height;
    data->width = width;

    queueEvent( event );
}

//Unattach OIS before window shutdown (very important under Linux)
void Engine::windowClosed( Ogre::RenderWindow* rw)
{
    queueEvent( EventPtr( new Event( "MSG_QUIT" ) ) );
}
