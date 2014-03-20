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
#include <iostream>
#include <exception>

#include "rendersystem.h"
//#include "inputsystem.h"
#include "scriptingsystem.h"
//#include "eventmanager.h"
//#include "luaconsole.h"
#include "gameentity.h"
//#include "animation.h"
//#include "Gorilla.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

using namespace std;
using namespace Ogre;

Root* root;
ScriptingSystem ss;

void test()
{
}

int main()
{
    cout << "Staring testing." << endl;

    root = new Root();

    root->restoreConfig();

    LuaResourceManager *luaRMgr = new LuaResourceManager();

    ResourceGroupManager::getSingleton().addResourceLocation( "../scripts", "FileSystem", "General" );

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    ss.initialise("TestingMain.lua");

    test();

    ss.shutdown();

    delete luaRMgr;
    delete root;

    return 0;
}

