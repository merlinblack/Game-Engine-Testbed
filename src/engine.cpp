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
#include <engine.h>
#include <inputeventdata.h>
#include <windoweventdata.h>
#include <OgreLogManager.h>
#include <luaresource.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <time.h>
void Sleep(int ms)
{
    struct timespec time;
    time.tv_sec = ms / 1000;
    time.tv_nsec = (ms % 1000) * (1000 * 1000);
    /*
    while( nanosleep(&time, &time) == -1 && errno == EINTR )
        continue; // We got woken by a system signal, go back to sleep.
    */
    // Since this function is only called to play 'nice' with the OS
    // we will just stop sleeping if we get a signal.
    nanosleep(&time, &time);
}
#else
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"	// For Sleep
#else
#error "Figure out what header for some sleep func, on your platform"
#endif
#endif

void bindLuaConsole( lua_State *L );    // From luaconsolebinding.cpp

Engine::~Engine()
{
    // Order of shutdown matters.
    animationManager    .shutdown();
    console             .shutdown();
    gameEntityManager   .shutdown();
    scriptingSystem     .shutdown();
    inputSystem         .shutdown();
    delete mGorilla;
    renderSystem        .shutdown();
}

bool Engine::initialise()
{
    // Managers may enque events as they start
    eventManager.addListener( this );
    eventManager.addListener( &renderSystem );
    eventManager.addListener( &scriptingSystem );
    eventManager.addListener( &inputSystem );
    eventManager.addListener( &animationManager );

    if( ! renderSystem.initialise() )
        return false;

    inputSystem.initialise( renderSystem.getWindow(), false );
    scriptingSystem.initialise();
    animationManager.initailise();

    renderSystem.addFrameListener( &scriptingSystem );

    // Init Gorilla
    mGorilla = new Gorilla::Silverback();
    mGorilla->loadAtlas("atlas");
    mGorillaScreen = mGorilla->createScreen( renderSystem.getViewport(), "atlas");

    console.init( mGorillaScreen, scriptingSystem.getInterpreter() );
    bindLuaConsole( scriptingSystem.getInterpreter() );

    gameEntityManager.initialise();

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
        animationManager.update();
        renderSystem.renderOneFrame();

        // Play nice with the operating system by sleeping a little.
        Sleep( 0 );
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
        RefCountedObjectPtr<InputEventData> data =
            static_cast<RefCountedObjectPtr<InputEventData> >( event->data );

        if( data->key == OIS::KC_F12 )   // Emergency stop.
        {
            stop = true;
            return true;
        }
        if( data->key == OIS::KC_GRAVE )
        {
            if( ! console.isVisible() || data->parm != '~' ) // If console is up, check for shift key
            {
                console.setVisible( ! console.isVisible() );
                return true;
            }
        }
        if( data->key == OIS::KC_NUMPADENTER )
        {
            // Map the Enter key on the keypad to Return on the main keyboard
            // Re-use old event and most of the data, reference counting will sort it out...
            data->key = OIS::KC_RETURN;

            queueEvent( event );

            return true; // We want the event taken out of the current queue.
        }
        if( console.isVisible() && data->key != OIS::KC_RETURN ) // See Keyup handling for why not RETURN key.
        {
            OIS::KeyEvent arg( 0, data->key, data->parm );
            console.injectKeyPress( arg );
            return true;
        }
        return false;
    }
    if( event->type == Event::hash( "EVT_KEYUP" ) )
    {
        RefCountedObjectPtr<InputEventData> data =
            static_cast<RefCountedObjectPtr<InputEventData> >( event->data );

        if( data->key == OIS::KC_NUMPADENTER )
        {
            // Map the Enter key on the keypad to Return on the main keyboard
            // Re-use old event and most of the data, reference counting will sort it out...
            data->key = OIS::KC_RETURN;

            queueEvent( event );

            return true; // We want the event taken out of the current queue.
        }
        if( console.isVisible() )
        {
            // Send RETURN key on the up rather than down event, so that any script run will not recieve the
            // up key press and possibly react to it.  For example the quit() script.
            // This avoids some scripts having to filter out RETURN key releases from when they where started
            // from the console.
            if( data->key == OIS::KC_RETURN )
            {
                OIS::KeyEvent arg( 0, data->key, data->parm );
                console.injectKeyPress( arg );
            }

            // Eat key up event.
            return true;
        }
        return false;
    }

    return false;
}

//Adjust mouse clipping area
void Engine::windowResized( Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int top, left;

    EventPtr event = newEvent( "EVT_WINDOW_RESIZE" );
    RefCountedObjectPtr<WindowEventData> data( new WindowEventData );

    rw->getMetrics(width, height, depth, left, top);

    event->data = data;
    data->height = height;
    data->width = width;

    queueEvent( event );
}

//Unattach OIS before window shutdown (very important under Linux)
void Engine::windowClosed( Ogre::RenderWindow* rw)
{
    queueEvent( EventPtr( newEvent( "MSG_QUIT" ) ) );
}
