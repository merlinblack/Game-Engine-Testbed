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
