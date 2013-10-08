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

#ifndef __ANIMATION_H
#define __ANIMATION_H

#include <eventmanager.h>

#include <OgrePrerequisites.h>
#include <OgreSingleton.h>
#include <OgreFrameListener.h>
#include <OgreVector3.h>

#include <vector>

#include <lua.hpp>
#include <LuaBridge.h>
#include <RefCountedObject.h>

#define DEFAULT_FADE_SPEED 0.5

class Animation : public RefCountedObject
{
    public:
        typedef RefCountedObjectPtr<Animation> Ptr;

        virtual void addTime( Ogre::Real timeSineLastFrame ) = 0;
        virtual bool isFinished() = 0;

        virtual void stop(){}
        virtual void start(){}

        virtual bool shouldRemoveWhenFinished()
        {
            return true;
        }

        virtual ~Animation()
        {
            stop();
        }

        bool operator==( const Animation::Ptr ptr )
        {
            return ptr == this;
        }
};

class MeshAnimation : public Animation
{
    Ogre::AnimationState* mState;
    Ogre::Real mFadeSpeed;
    bool mFadingIn;
    bool mFadingOut;
    bool mRemoveWhenFinished;

    public:
    MeshAnimation( Ogre::Entity* ent, Ogre::String name );

    void addTime( Ogre::Real timeSinceLastFrame );
    void setWeighting( Ogre::Real weighting );
    void setFadeSpeed( Ogre::Real percentWeightPerSecond = DEFAULT_FADE_SPEED );

    void fadeIn();
    void fadeOut();
    bool isFadingIn();
    bool isFadingOut();

    void setLoop( bool loop );

    void stop();
    void start();

    inline bool isFinished();

    inline bool shouldRemoveWhenFinished();
    inline void setRemoveWhenFinished( bool shouldRemoveWhenFinished );
};

class MovementAnimation : public Animation
{
    Ogre::SceneNode* mNode;
    Ogre::Vector3 mDestination;
    Ogre::Real mSpeed;
    bool mMoving;

    public:
    MovementAnimation( Ogre::SceneNode* nodeToMove, Ogre::Vector3 destination, Ogre::Real speed );

    void addTime( Ogre::Real timeSinceLastFrame );

    void stop();
    void start();

    inline bool isFinished();
};

class RotationAnimation : public Animation
{
    Ogre::SceneNode* mNode;
    Ogre::Quaternion mInitialOrientation;
    Ogre::Quaternion mDestOrientation;
    Ogre::Real mProgress;
    Ogre::Real mSpeed;

    bool mRotating;
    public:
    RotationAnimation( Ogre::SceneNode* nodeToRotate, Ogre::Quaternion rotation, Ogre::Real speed );

    void addTime( Ogre::Real timeSinceLastFrame );

    void stop();
    void start();

    inline bool isFinished();
};

class AnimationManager :
    public Ogre::Singleton<AnimationManager>,
    public Ogre::FrameListener,
    public EventListenerSender
{
    std::vector<Animation::Ptr> animations;
    Ogre::Real timeSinceLastFrame;
    const size_t finishEvent;

    public:
    AnimationManager();
    ~AnimationManager();

    void initailise();
    void shutdown();

    void addAnimation( Animation::Ptr animation );
    void removeAnimation( Animation::Ptr animation );
    void update();

    static AnimationManager& getSingleton();
    static AnimationManager* getSingletonPtr();

    bool frameStarted( const Ogre::FrameEvent& evt );

    bool EventNotification( EventPtr event )
    { return false; }
};

#endif // __ANIMATION_H
