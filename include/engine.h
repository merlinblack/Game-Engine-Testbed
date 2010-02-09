#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "rendersystem.h"
#include "inputsystem.h"
#include "scriptingsystem.h"
#include "eventmanager.h"
#include "luaconsole.h"
#include "gameentity.h"

class Engine : public EventListenerSender, public Ogre::WindowEventListener
{
    RenderSystem renderSystem;
    InputSystem inputSystem;
    ScriptingSystem scriptingSystem;
    EventManager eventManager;
    LuaConsole console;
    GameEntityManager gameEntityManager;
    bool stop;

public:
    virtual ~Engine();
    bool initialise();
    void run();

    bool EventNotification( EventPtr event );

    void windowResized(Ogre::RenderWindow* rw);
    void windowClosed(Ogre::RenderWindow* rw);

};

#endif // ENGINE_H_INCLUDED
