#ifndef __ANIMATION_H
#define __ANIMATION_H

#include <eventmanager.h>

#include <OgrePrerequisites.h>
#include <OgreSingleton.h>
#include <OgreFrameListener.h>
#include <OgreVector3.h>

#include <vector>
#include <boost/shared_ptr.hpp>

#define DEFAULT_FADE_SPEED 0.5

class Animation
{
    public:
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
};

class MeshAnimation : public Animation
{
    Ogre::AnimationState* mState;
    Ogre::Real mFadeSpeed;
    bool mFadingIn;
    bool mFadingOut;
    bool mRemoveWhenFinished;

    public:
    MeshAnimation( Ogre::Entity* ent, Ogre::String& name );

    void addTime( Ogre::Real timeSinceLastFrame );
    void setWeighting( Ogre::Real weighting );
    void setFadeSpeed( Ogre::Real percentWeightPerSecond 
            = DEFAULT_FADE_SPEED );

    void fadeIn();
    void fadeOut();
    inline bool isFadingIn();
    inline bool isFadingOut();

    inline void setLoop( bool loop );

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

typedef boost::shared_ptr<Animation> AnimationPtr;

class AnimationManager : 
    public Ogre::Singleton<AnimationManager>,
    public Ogre::FrameListener,
    public EventListenerSender
{
    std::vector<AnimationPtr> animations;
    Ogre::Real timeSinceLastFrame;
    const size_t finishEvent;

    public:
    AnimationManager();
    ~AnimationManager();

    void initailise();
    void shutdown();

    void addAnimation( AnimationPtr animation );
    void removeAnimation( AnimationPtr animation );
    void update();

    static AnimationManager& getSingleton();
    static AnimationManager* getSingletonPtr();

    bool FrameStarted( Ogre::FrameEvent& evt );
    bool FrameEnded( Ogre::FrameEvent& evt );

    bool EventNotification( EventPtr event )
    { return false; }
};

#endif // __ANIMATION_H
