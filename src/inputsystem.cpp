#include <inputsystem.h>

void InputSystem::shutdown()
{
    if( mInputManager )
    {
        if( mMouse )
        {
            mInputManager->destroyInputObject( mMouse );
            mMouse = 0;
        }

        if( mKeyboard )
        {
            mInputManager->destroyInputObject( mKeyboard );
            mKeyboard = 0;
        }

        OIS::InputManager::destroyInputSystem( mInputManager );
        mInputManager = 0;
    }
}

void InputSystem::initialise( Ogre::RenderWindow *window, bool exclusive )
{
    OIS::ParamList paramList;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    if( ! exclusive )
    {
        #if defined OIS_WIN32_PLATFORM
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
        #elif defined OIS_LINUX_PLATFORM
        paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
        paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
        #endif
    }

    mInputManager = OIS::InputManager::createInputSystem( paramList );

    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));

    mKeyboard->setEventCallback( this );

    adjustWindowSize( window );

    // Cache hash values for creating events.
    event_keydown = Event::hash( "EVT_KEYDOWN" );
    event_keyup = Event::hash( "EVT_KEYUP" );
}

void InputSystem::adjustWindowSize( Ogre::RenderWindow *window )
{
    unsigned int width, height, depth;
    int top, left;
    window->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputSystem::capture()
{
    mMouse->capture();
    mKeyboard->capture();
}

bool InputSystem::keyPressed( const OIS::KeyEvent &arg )
{
    EventPtr event( new Event( event_keydown ) );

    event->key = arg.key;
    event->param1 = arg.text;

    boost::shared_ptr<InputEventData> data( new InputEventData );

    data->key = arg.key;
    data->parm = arg.text;

    event->data = data;

    queueEvent( event );

    return true;
}

bool InputSystem::keyReleased( const OIS::KeyEvent &arg )
{
    EventPtr event( new Event( event_keyup ) );

    event->key = arg.key;

    queueEvent( event );

    return true;
}
