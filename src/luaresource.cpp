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

/** @brief LuaResource
  *
  * @todo: document this function
  */
LuaResource::LuaResource(Ogre::ResourceManager* creator, const Ogre::String& name,
    Ogre::ResourceHandle handle, const Ogre::String& group ,
    bool isManual , Ogre::ManualResourceLoader* loader) :
    Ogre::Resource( creator, name, handle, group, isManual, loader )
{
    createParamDictionary("LuaResource");
}

/** @brief ~LuaResource
  *
  * @todo: document this function
  */
LuaResource::~LuaResource()
{
    unload();
}

/** @brief calculateSize
  *
  * @todo: document this function
  */
size_t LuaResource::calculateSize() const
{
    return mScriptSource.length();
}

/** @brief unloadImpl
  *
  * @todo: document this function
  */
void LuaResource::unloadImpl()
{
    mScriptSource.clear();
}

/** @brief loadImpl
  *
  * @todo: document this function
  */
void LuaResource::loadImpl()
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton()
        .openResource( mName, mGroup, true, this );

    mScriptSource = stream->getAsString();
}

/** @brief getScriptSource
  *
  * @todo: document this function
  */
const Ogre::String LuaResource::getScriptSource() const
{
    return mScriptSource;
}

template<>
LuaResourceManager* Ogre::Singleton<LuaResourceManager>::ms_Singleton = 0;

/** @brief LuaResourceManager
  *
  * @todo: document this function
  */
LuaResourceManager::LuaResourceManager()
{
    mResourceType = "LuaScript";
    mLoadOrder = 50.0f;

    // Register with OGRE
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager( mResourceType, this );
}

/** @brief ~LuaResourceManager
  *
  * @todo: document this function
  */
LuaResourceManager::~LuaResourceManager()
{
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

/** @brief createImpl
  *
  * @todo: document this function
  */
Ogre::Resource* LuaResourceManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams)
{
    return new LuaResource( this, name, handle, group, isManual, loader );
}

/** @brief load
  *
  * @todo: document this function
  */
LuaResourcePtr LuaResourceManager::load(const Ogre::String& name, const Ogre::String& group)
{
    LuaResourcePtr script = getByName(name);

    if( script.isNull() )   // Not yet created
        script = create( name, group );

    script->load();

    return script;
}

/** @brief getSingeltonPtr
  *
  * @todo: document this function
  */
LuaResourceManager * LuaResourceManager::getSingeltonPtr()
{
    return ms_Singleton;
}

/** @brief getSingleton
  *
  * @todo: document this function
  */
LuaResourceManager & LuaResourceManager::getSingleton()
{
    assert(ms_Singleton);
    return *ms_Singleton;
}

