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
#ifndef LUARESOURCE_H
#define LUARESOURCE_H

#include <OgreResourceManager.h>

class LuaResource : public Ogre::Resource
{
    Ogre::String mScriptSource;

protected:
    void loadImpl();
    void unloadImpl();

public:

    size_t calculateSize() const;

    LuaResource(Ogre::ResourceManager* creator, const Ogre::String& name,
                Ogre::ResourceHandle handle,
                const Ogre::String& group = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

    virtual ~LuaResource();

    const Ogre::String getScriptSource() const;
};

class LuaResourcePtr : public Ogre::SharedPtr<LuaResource>
{
public:
    LuaResourcePtr() : Ogre::SharedPtr<LuaResource>() {}
    explicit LuaResourcePtr( LuaResource* p ) : Ogre::SharedPtr<LuaResource>(p) {}
    LuaResourcePtr( const LuaResourcePtr& r ) : Ogre::SharedPtr<LuaResource>(r) {}

    LuaResourcePtr( const Ogre::ResourcePtr& r ) : Ogre::SharedPtr<LuaResource>()
    {
        if( r.isNull() )
            return; // Don't copy, parent constructor already done enough.

        // Lock and copy other mutex pointer.
        OGRE_LOCK_MUTEX( *r.OGRE_AUTO_MUTEX_NAME )
        OGRE_COPY_AUTO_SHARED_MUTEX( r.OGRE_AUTO_MUTEX_NAME )

        // Copy pointer, pointer to ref count, and memory free'ing stratagy
        pRep = static_cast<LuaResource*>( r.getPointer() );
        pUseCount = r.useCountPointer();
        useFreeMethod = r.freeMethod();

        // Bump reference count
        if( pUseCount )
            (*pUseCount)++;
    }

    LuaResourcePtr& operator=( const Ogre::ResourcePtr& r )
    {
        if( pRep == static_cast<LuaResource*>( r.getPointer() ) )
            return *this; // Already pointing at the same object.

        release();

        if( r.isNull() )
            return *this; // Don't copy, release() already done enough.

        // Lock and copy other mutex pointer.
        OGRE_LOCK_MUTEX( *r.OGRE_AUTO_MUTEX_NAME )
        OGRE_COPY_AUTO_SHARED_MUTEX( r.OGRE_AUTO_MUTEX_NAME )

        // Copy pointer, pointer to ref count, and memory free'ing stratagy
        pRep = static_cast<LuaResource*>( r.getPointer() );
        pUseCount = r.useCountPointer();
        useFreeMethod = r.freeMethod();

        // Bump reference count
        if( pUseCount )
            (*pUseCount)++;

        return *this;
    }
};

class LuaResourceManager : public Ogre::ResourceManager, public Ogre::Singleton<LuaResourceManager>
{
protected:
    Ogre::Resource *createImpl( const Ogre::String& name, Ogre::ResourceHandle handle,
        const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
        const Ogre::NameValuePairList* createParams);

public:
    LuaResourceManager();
    virtual ~LuaResourceManager();

    virtual LuaResourcePtr load( const Ogre::String& name, const Ogre::String& group = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

    static LuaResourceManager& getSingleton();
    static LuaResourceManager* getSingeltonPtr();
};

#endif // LUARESOURCE_H

