#ifndef SCRIPTINGSYSTEM_H_INCLUDED
#define SCRIPTINGSYSTEM_H_INCLUDED

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <eventmanager.h>
#include <OgreFrameListener.h>

class ScriptingSystem : public EventListenerSender, public Ogre::FrameListener
{
    lua_State *mL;
    static int GUID;

public:
    friend void queueEventThunk( lua_State *, EventPtr );

    ScriptingSystem() : mL(0)
    {
    }
    ~ScriptingSystem()
    {
        shutdown();
    }

    void shutdown();
    void initialise();
    void bind();

    bool EventNotification( EventPtr event );

    bool frameStarted(const Ogre::FrameEvent& evt);
    bool frameEnded(const Ogre::FrameEvent& evt);

    lua_State* getInterpreter() { return mL; }
};

#endif // SCRIPTINGSYSTEM_H_INCLUDED
