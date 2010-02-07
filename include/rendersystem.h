#ifndef RENDERSYSTEM_H_INCLUDED
#define RENDERSYSTEM_H_INCLUDED

#include <Ogre.h>
#include <luaresource.h>
#include <eventmanager.h>

class RenderSystem : public EventListenerSender
{
    Ogre::Root* mRoot;
    Ogre::SceneManager* mSceneManager;
    Ogre::RenderWindow* mWindow;
    Ogre::Camera* mCamera;
    LuaResourceManager* mLuaResourceManager;

public:
    RenderSystem() : mRoot(0), mSceneManager(0), mWindow(0), mCamera(0), mLuaResourceManager(0)
    {
    }
    ~RenderSystem()
    {
        shutdown();
    }

    void shutdown();
    bool initialise();

    Ogre::RenderWindow* getWindow() { return mWindow; }
    Ogre::Root* getRoot() { return mRoot; }
    void addFrameListener( Ogre::FrameListener *listener ) { mRoot->addFrameListener( listener ); }

    void renderOneFrame();
    void addResourceLocations();

    bool EventNotification( EventPtr event )
    {
        return false;
    }
};

#endif // RENDERSYSTEM_H_INCLUDED
