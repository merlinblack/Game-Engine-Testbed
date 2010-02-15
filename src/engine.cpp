#include <engine.h>
#include <inputeventdata.h>
#include <windoweventdata.h>
#include <OgreLogManager.h>
#include <luaresource.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <time.h>
void msleep(int ms)
{
    struct timespec time;
    time.tv_sec = ms / 1000;
    time.tv_nsec = (ms % 1000) * (1000 * 1000);
    while( nanosleep(&time, &time) == -1 && errno == EINTR )
        continue; // We got woken by a system signal, go back to sleep
}
#else
#error "Figure out what header for some sleep func, on your platform"
#endif

void bindLuaConsole( lua_State *L );    // From luaconsolebinding.cpp

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

    return true;
}

void Engine::run()
{
    while( stop == false )
    {
        Ogre::WindowEventUtilities::messagePump();
        inputSystem.capture();
        eventManager.processEvents();
        gameEntityManager.update();
        renderSystem.renderOneFrame();

        // Play nice with the operating system by sleeping a little.
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        msleep( 10 );
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        sleep( ??? );   // Might be the same as Linux.
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        Sleep( 10 );    // TODO: Check this func takes milliseconds!
#endif

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
