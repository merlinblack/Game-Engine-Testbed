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
#include <rendersystem.h>
#include <windoweventdata.h>

using namespace Ogre;

void ::RenderSystem::shutdown()
{
    if( mRoot )
    {
        delete mLuaResourceManager;
        delete mRoot;
        mSceneManager = 0;
        mWindow   = 0;
        mCamera   = 0;
        mViewport = 0;
        mRoot     = 0;
    }
}

bool ::RenderSystem::initialise()
{
    try
    {
        mRoot = new Root();
    }
    catch( Ogre::Exception &e )
    {
        std::cout << e.what() << std::endl;
        return false;
    }

    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        try
        {
            mWindow = mRoot->initialise(true);
        }
        catch( Ogre::Exception &e )
        {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }

    // Create the SceneManager, in this case a generic one
    mSceneManager = mRoot->createSceneManager(ST_GENERIC, "SceneManagerInstance");

    // Create the camera
    mCamera = mSceneManager->createCamera("MainCamera");

    // Create one viewport, entire window
    mViewport = mWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(ColourValue(0,0,1));

    mLuaResourceManager = new LuaResourceManager();

    addResourceLocations();

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Notify other systems of the window size.
    unsigned int width, height, depth;
    int top, left;
    EventPtr event = newEvent( "EVT_WINDOW_RESIZE" );
    RefCountedObjectPtr<WindowEventData> data( new WindowEventData );
    mWindow->getMetrics(width, height, depth, left, top);
    event->data = data;
    data->height = height;
    data->width = width;
    queueEvent( event );

    // Setup a small test scene.
    mSceneManager->setSkyBox( true, "StormySkyBox" );

    mSceneManager->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) );
    mSceneManager->setShadowTechnique( SHADOWTYPE_STENCIL_MODULATIVE );

    /*
    Light* light = mSceneManager->createLight( "Light1" );
    light->setDiffuseColour( 1.0, 1.0, 1.0 );
    light->setSpecularColour( 1.0, 1.0, 1.0 );
    light->setPosition( -50, 200, 50 );
    */

    return true;
}

void ::RenderSystem::renderOneFrame()
{
    mRoot->renderOneFrame();
}

bool ::RenderSystem::EventNotification( EventPtr event )
{
    if( event->type == Event::hash( "EVT_WINDOW_RESIZE" ) )
    {
        RefCountedObjectPtr<WindowEventData> data =
            static_cast<RefCountedObjectPtr<WindowEventData> >( event->data );

        // Alter the camera aspect ratio
        mCamera->setAspectRatio(Real(data->width) / Real(data->height));
    }
    return false;
}

void ::RenderSystem::addResourceLocations()
{
   // Load resource paths from config file
   ConfigFile cf;
   cf.load("media.cfg");

   // Go through all sections & settings in the file
   ConfigFile::SectionIterator seci = cf.getSectionIterator();

   String secName, typeName, archName;
   while (seci.hasMoreElements())
   {
      secName = seci.peekNextKey();
      ConfigFile::SettingsMultiMap *settings = seci.getNext();
      ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
         typeName = i->first;
         archName = i->second;
         ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
      }
   }
}

