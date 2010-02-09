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
        mWindow = 0;
        mCamera = 0;
        mRoot   = 0;
    }
}

bool ::RenderSystem::initialise()
{
    mRoot = new Root();

    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true);
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
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,1));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    mLuaResourceManager = new LuaResourceManager();

    addResourceLocations();

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Notify other systems of the window size.
    unsigned int width, height, depth;
    int top, left;
    EventPtr event( new Event( "EVT_WINDOW_RESIZE" ) );
    boost::shared_ptr<WindowEventData> data( new WindowEventData );
    mWindow->getMetrics(width, height, depth, left, top);
    event->data = data;
    data->height = height;
    data->width = width;
    queueEvent( event );

    // Setup a small test scene.
    mSceneManager->setSkyBox( true, "CloudyNoonSkyBox" );

    return true;
}

void ::RenderSystem::renderOneFrame()
{
    mRoot->renderOneFrame();
}

//-------------------------------------------------------------------------------------
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

