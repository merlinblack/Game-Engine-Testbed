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

#include <lua.hpp>
#include <OgrePrerequisites.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <git_versioning.h>
#include <LuaBridge.h>

Ogre::String versionString()
{
    Ogre::String version;

    version = "Game Engine Testbed\n";
    version += GIT_REPO_VERSION_STR;
    version += "\n";

    version += "Ogre ";
    version += Ogre::StringConverter::toString( OGRE_VERSION_MAJOR );
    version += ".";
    version += Ogre::StringConverter::toString( OGRE_VERSION_MINOR );
    version += ".";
    version += Ogre::StringConverter::toString( OGRE_VERSION_PATCH );
    version += " ";
    version += OGRE_VERSION_NAME;
    version += " ";
    version += OGRE_VERSION_SUFFIX;
    version += "\n";

    version += LUA_RELEASE;
    version += "\n";

    version += "LuaBridge ";
    version += Ogre::StringConverter::toString( LUABRIDGE_MAJOR_VERSION );
    version += ".";
    version += Ogre::StringConverter::toString( LUABRIDGE_MINOR_VERSION );
    version += "\n";

    return version;
}
