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
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "rendersystem.h"
#include "inputsystem.h"
#include "scriptingsystem.h"
#include "eventmanager.h"
#include "luaconsole.h"
#include "gameentity.h"
#include "animation.h"
#include "Gorilla.h"

class Engine : public EventListenerSender, public Ogre::WindowEventListener
{
    RenderSystem renderSystem;
    InputSystem inputSystem;
    ScriptingSystem scriptingSystem;
    EventManager eventManager;
    LuaConsole console;
    GameEntityManager gameEntityManager;
    AnimationManager animationManager;
    Gorilla::Silverback *mGorilla;
    Gorilla::Screen *mGorillaScreen;
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
