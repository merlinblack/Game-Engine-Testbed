#include <engine.h>

void BindLuaConsole( lua_State *L );	// From luaconsolebinding.cpp

bool Engine::initialise()
{
    if( ! renderSystem.initialise() )
        return false;

    inputSystem.initialise( renderSystem.getWindow(), false );
    scriptingSystem.initialise();

    eventManager.addListener( this );
    eventManager.addListener( &scriptingSystem );
    eventManager.addListener( &inputSystem );

    renderSystem.addFrameListener( &scriptingSystem );

    console.init( renderSystem.getRoot(), scriptingSystem.getInterpreter() );
	BindLuaConsole( scriptingSystem.getInterpreter() );

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

    if( event->type == Event::hash( "EVT_KEYUP" ) )
    {
        if( event->key == OIS::KC_F12 )   // Emergency stop.
        {
            stop = true;
            return true;
        }
    }

    if( event->type == Event::hash( "EVT_KEYDOWN" ) )
    {
        if( event->key == OIS::KC_GRAVE )
        {
            console.setVisible( ! console.isVisible() );
            return true;
        }
        if( console.isVisible() )
        {
            OIS::KeyEvent arg( 0, event->key, event->param1 );
            console.injectKeyPress( arg );
            return true;
        }
    }

    return false;
}

//Adjust mouse clipping area
void Engine::windowResized( Ogre::RenderWindow* rw)
{
    inputSystem.adjustWindowSize( rw );
}

//Unattach OIS before window shutdown (very important under Linux)
void Engine::windowClosed( Ogre::RenderWindow* rw)
{
    queueEvent( EventPtr( new Event( "MSG_QUIT" ) ) );
}
