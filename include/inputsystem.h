#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

#include <OIS/OIS.h>
#include <OgreRenderWindow.h>
#include <eventmanager.h>

class InputSystem : public EventListenerSender, public OIS::KeyListener
{
    OIS::InputManager *mInputManager;
    OIS::Mouse *mMouse;
    OIS::Keyboard *mKeyboard;
    std::size_t event_keydown, event_keyup;

public:
    InputSystem() : mInputManager(0), mMouse(0), mKeyboard(0)
    {
    }

    ~InputSystem()
    {
        shutdown();
    }

    void shutdown();
    void initialise( Ogre::RenderWindow *window, bool exclusive );
    void adjustWindowSize( Ogre::RenderWindow *window );

    void capture();

    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );

    bool EventNotification( EventPtr event )
    {
        return false;
    }
};

#endif // INPUTSYSTEM_H_INCLUDED
