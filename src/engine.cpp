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

//For testing
#include <navigationmesh.h>

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
#error "Figure out what header for some sleep func, on your platform"
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

    console.init( renderSystem.getRoot(), scriptingSystem.getInterpreter() );
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
        // Sleep( 10 );
    }
}

bool Engine::EventNotification( EventPtr event )
{
    // These are for testing. TOREMOVE!
    static float _X;
    static float _Y;
    static short _WIDTH;
    static short _HEIGHT;

    if( event->type == Event::hash( "MSG_QUIT" ) )
    {
        stop = true;
        return true;
    }

    if( event->type == Event::hash( "EVT_WINDOW_RESIZE" ) )
    {
        boost::shared_ptr<WindowEventData> data = boost::dynamic_pointer_cast<WindowEventData>( event->data );
        _WIDTH = data->width;
        _HEIGHT = data->height;

        return false;   // We don't eat this event as other listeners may need to know too.
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
            OIS::KeyEvent arg( 0, data->key, data->parm );
            console.injectKeyPress( arg );
            return true;
        }
        if( data->key == OIS::KC_T )    // T for Test!
        {
            // Testing.
            Ogre::Root *root = Ogre::Root::getSingletonPtr();
            Ogre::SceneManager* mgr = root->getSceneManager( "SceneManagerInstance" );
            Ogre::Entity *eTest = mgr->createEntity( "FloorNav.mesh" );

            NavigationMesh navMesh;

            navMesh.BuildFromOgreMesh( eTest->getMesh() );

            mgr->destroyEntity( eTest );

            // Get where the mouse is pointing on the floor
            GameEntityPtr base = gameEntityManager.getGameEntity( "Floor" );

            if( !base ) // test scene not yet loaded.
            {
                Ogre::LogManager::getSingleton().stream() << "Load the test scene first you doofus.";
                return true;
            }

            Ogre::Vector3 pos = base->hitPosition( _X, _Y );

            if( pos.x == Ogre::Math::POS_INFINITY )
                return true;    // Mouse was not over the Floor.

            // Test out pathfinding.
            NavigationPath* path = navMesh.findNavigationPath( Ogre::Vector3( 0, 25.6, 0 ), pos );

            mgr->destroyManualObject( "path" );

            if( path )
            {
                const Ogre::Real width = 5.0;

                NavigationPath *straightenedPath = navMesh.straightenPath( path, Ogre::Radian( Ogre::Math::PI/2 ), width );

                Ogre::SceneNode* myManualObjectNode;

                if( mgr->hasSceneNode( "path_node" ) )
                    myManualObjectNode = mgr->getSceneNode( "path_node" );
                else
                    myManualObjectNode = mgr->getRootSceneNode()->createChildSceneNode("path_node");

                Ogre::ManualObject* myManualObject =  mgr->createManualObject("path");

                myManualObject->begin("debug/yellow", Ogre::RenderOperation::OT_LINE_STRIP);

                myManualObject->position(*(path->begin()));
                for( NavigationPath::iterator i = path->begin() + 1; i != path->end(); i++ )
                {
                    Ogre::Vector3 tick;
                    Ogre::Vector3 direction = *i - *(i-1);
                    Ogre::Vector3 rightAngle( direction.z, 0, -direction.x );
                    rightAngle.normalise();

                    myManualObject->position(i->x, i->y+2, i->z);

                    tick = *i + rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2, i->z);

                    tick = *i - rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2, i->z);
                }

                myManualObject->end();

                myManualObject->begin("debug/cyan", Ogre::RenderOperation::OT_LINE_STRIP);

                myManualObject->position(*(straightenedPath->begin()));
                for( NavigationPath::iterator i = straightenedPath->begin() + 1; i != straightenedPath->end(); i++ )
                {
                    std::cout << *i << std::endl;

                    Ogre::Vector3 tick;
                    Ogre::Vector3 direction = *i - *(i-1);
                    Ogre::Vector3 rightAngle( direction.z, 0, -direction.x );
                    rightAngle.normalise();

                    myManualObject->position(i->x, i->y+2.5, i->z);

                    tick = *i + rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2.5, i->z);

                    tick = *i - rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2.5, i->z);
                }

                myManualObject->end();

                myManualObject->setQueryFlags( 0 );
                myManualObject->setRenderQueueGroup( Ogre::RENDER_QUEUE_OVERLAY );

                myManualObjectNode->attachObject(myManualObject);

                delete path;
                delete straightenedPath;
            }

            //navMesh.DebugTextDump( std::cout );

            return true;
        }
        if( data->key == OIS::KC_Y )    // Yet another test.
        {
            // Get the robot and animate him.

            GameEntityPtr robot = gameEntityManager.getGameEntity( "Mike" );

            if( !robot ) // test scene not yet loaded.
            {
                Ogre::LogManager::getSingleton().stream() << "Load the test scene first you doofus.";
                return true;
            }

            MeshAnimationPtr anim( new MeshAnimation( robot->mesh, "Idle" ) );
            AnimationManager::getSingleton().addAnimation( anim );
            anim->setWeighting( 0.0f );
            anim->fadeIn();
            anim->start();
        }
        if( data->key == OIS::KC_U )
        {
            // Get the robot and animate him.

            GameEntityPtr robot = gameEntityManager.getGameEntity( "Mike" );

            if( !robot ) // test scene not yet loaded.
            {
                Ogre::LogManager::getSingleton().stream() << "Load the test scene first you doofus.";
                return true;
            }

            MovementAnimationPtr anim( new MovementAnimation( robot->sceneNode,
                                                              robot->sceneNode->getPosition() + Ogre::Vector3( 10, 0, 0 ),
                                                              10.0f ) );

            AnimationManager::getSingleton().addAnimation( anim );
            anim->start();

            Ogre::Quaternion q( Ogre::Degree(180), Ogre::Vector3::UNIT_Y);

            RotationAnimationPtr anim2( new RotationAnimation( robot->sceneNode, q, 2 ));

            AnimationManager::getSingleton().addAnimation( anim2 );
            anim2->start();
        }
    }

    if( event->type == Event::hash("EVT_MOUSEMOVE") )
    {
        boost::shared_ptr<InputEventData> data = boost::dynamic_pointer_cast<InputEventData>( event->data );

        _X = (float)data->x / _WIDTH;
        _Y = (float)data->y / _HEIGHT;
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
