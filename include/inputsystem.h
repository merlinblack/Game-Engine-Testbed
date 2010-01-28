#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

#include <OIS/OIS.h>
#include <OgreRenderWindow.h>
#include <eventmanager.h>

class InputSystem : public EventListenerSender, public OIS::KeyListener, public OIS::MouseListener
{
    OIS::InputManager *mInputManager;
    OIS::Mouse *mMouse;
    OIS::Keyboard *mKeyboard;
    std::size_t event_keydown, event_keyup;
    std::size_t event_mousedown, event_mouseup, event_mousemove;

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

    bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    bool EventNotification( EventPtr event )
    {
        return false;
    }
};

#endif // INPUTSYSTEM_H_INCLUDED
