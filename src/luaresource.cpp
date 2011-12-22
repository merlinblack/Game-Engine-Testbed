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
#include "luaresource.h"

LuaResource::LuaResource(Ogre::ResourceManager* creator, const Ogre::String& name,
    Ogre::ResourceHandle handle, const Ogre::String& group ,
    bool isManual , Ogre::ManualResourceLoader* loader) :
    Ogre::Resource( creator, name, handle, group, isManual, loader )
{
    createParamDictionary("LuaResource");
}

LuaResource::~LuaResource()
{
    unload();
}

size_t LuaResource::calculateSize() const
{
    return mScriptSource.length();
}

void LuaResource::unloadImpl()
{
    mScriptSource.clear();
}

void LuaResource::loadImpl()
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton()
        .openResource( mName, mGroup, true, this );

    mScriptSource = stream->getAsString();
}

const Ogre::String LuaResource::getScriptSource() const
{
    return mScriptSource;
}

#if OGRE_VERSION < 67584 /* 1.8.0 */
template<>
LuaResourceManager* Ogre::Singleton<LuaResourceManager>::ms_Singleton = 0;
#else
template<>
LuaResourceManager* Ogre::Singleton<LuaResourceManager>::msSingleton = 0;
#endif

LuaResourceManager::LuaResourceManager()
{
    mResourceType = "LuaScript";
    mLoadOrder = 50.0f;

    // Register with OGRE
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager( mResourceType, this );
}

LuaResourceManager::~LuaResourceManager()
{
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

Ogre::Resource* LuaResourceManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams)
{
    return new LuaResource( this, name, handle, group, isManual, loader );
}

LuaResourcePtr LuaResourceManager::load(const Ogre::String& name, const Ogre::String& group)
{
    LuaResourcePtr script = getByName(name);

    if( script.isNull() )   // Not yet created
        script = create( name, group );

    script->load();

    return script;
}

#if OGRE_VERSION < 67584 /* 1.8.0 */
LuaResourceManager * LuaResourceManager::getSingeltonPtr()
{
    return ms_Singleton;
}

LuaResourceManager & LuaResourceManager::getSingleton()
{
    assert(ms_Singleton);
    return *ms_Singleton;
}
#else
LuaResourceManager * LuaResourceManager::getSingeltonPtr()
{
    return msSingleton;
}

LuaResourceManager & LuaResourceManager::getSingleton()
{
    assert(msSingleton);
    return *msSingleton;
}
#endif

