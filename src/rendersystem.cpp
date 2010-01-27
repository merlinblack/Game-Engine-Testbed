#include <rendersystem.h>

using namespace Ogre;

void ::RenderSystem::shutdown()
{
    if( mRoot )
    {
        delete mRoot;
        mSceneManager = 0;
        mWindow = 0;
        mCamera = 0;
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
    mSceneManager = mRoot->createSceneManager(ST_GENERIC);

    // Create the camera
    mCamera = mSceneManager->createCamera("MainCamera");

    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,1));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    ResourceGroupManager::getSingleton().addResourceLocation("media", "FileSystem", "General");
    ResourceGroupManager::getSingleton().addResourceLocation("media/cubemapsJS.zip", "Zip", "General");

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup a small test scene.
    mSceneManager->setSkyBox( true, "EveningSkyBox" );

    return true;
}

void ::RenderSystem::renderOneFrame()
{
    mRoot->renderOneFrame();
}

