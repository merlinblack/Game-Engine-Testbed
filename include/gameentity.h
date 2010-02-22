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
#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSingleton.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <luabind/luabind.hpp>

class GameEntity;
class GameEntityManager;

typedef boost::shared_ptr<GameEntity> GameEntityPtr;

class GameEntity
{
    friend class GameEntityManager;

    // Name
    std::string name;
    // HashID
    size_t hashId;

public: // TODO: Make private and add accessor funcs.

    GameEntityPtr parent;

    // Scene Node
    Ogre::SceneNode* sceneNode;
    // Entity
    Ogre::Entity* mesh;

    // Physics Shape
    // btSomethingErRather* physical;

public:
    GameEntity();
    virtual ~GameEntity();

    //Accessors
    std::string getName() { return name; }
    void setName( std::string newName );

    size_t getHashId() { return hashId; }

    virtual void update();
    void collide( GameEntityPtr other );

    static boost::hash<std::string> hasher;
    size_t hash( std::string str )
    {
        return hasher(str);
    }
};

// This allows GameEntities past from Lua to C++ and back
// to retain thier Lua parts.  In other words, not get sliced.
// Also Lua can derive from GameEntity and override the update
// method.
class GameEntityWrapper : public GameEntity, public luabind::wrap_base
{
    public:

    virtual void update()
    {
        call<void>( "update" );
    }

    static void default_update( GameEntity* ptr )
    {
        ptr->GameEntity::update();
    }
};

class GameEntityManager : public Ogre::Singleton<GameEntityManager>
{
    typedef std::pair<size_t, GameEntityPtr> mapping;
    
    std::map<size_t, GameEntityPtr> entities;
public:
    static GameEntityManager* getSingletonPtr();
    static GameEntityManager& getSingleton();

    bool addGameEntity( GameEntityPtr p );
    void removeGameEntity( size_t hashId );
    void removeGameEntity( std::string name );
    GameEntityPtr getGameEntity( size_t hashId );
    GameEntityPtr getGameEntity( std::string name );

    void update();
};

void bindGameEntityClasses( lua_State* L );

#endif // GAMEENTITY_H
