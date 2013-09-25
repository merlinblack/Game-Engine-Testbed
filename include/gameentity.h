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

#include <list>
#include <string>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSingleton.h>
#include <OgreLogManager.h>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <lua.hpp>
#include <LuaBridge.h>
#include <RefCountedObject.h>

class GameEntity;
class GameEntityManager;

class GameEntity : public RefCountedObject
{
    friend class GameEntityManager;

    // Name
    std::string name;
    // HashID
    size_t hashId;
    // Original material
    Ogre::MaterialPtr originalMaterial;
    // Material used for highlight.
    Ogre::MaterialPtr highlightMaterial;

public: // TODO: Make private and add accessor funcs.

    RefCountedObjectPtr<GameEntity> parent;

    // Scene Node
    Ogre::SceneNode* sceneNode;
    // Entity
    Ogre::Entity* mesh;

    // Physics Shape
    // btSomethingErRather* physical;

public:
    GameEntity( lua_State* L);
    virtual ~GameEntity();

    // Lua function for updates
    luabridge::LuaRef update_overide;

    // Accessors
    std::string getName() const { return name; }
    void setName( std::string newName );

    size_t getHashId() { return hashId; }

    virtual void update();

    // If this instance is being managed, it is removed.
    void removeFromManager();

    // Checks if a ray from camera at (x,y) intersects with *any* mesh triangles.
    // Returns as soon as an intersection is found.
    bool hitCheck( float x, float y );
    // Returns the position of intersection with the mesh triangle that results in
    // the smallest distance from the camera.
    Ogre::Vector3 hitPosition( float x, float y );

    static boost::hash<std::string> hasher;
    size_t hash( std::string str )
    {
        return hasher(str);
    }
    bool operator==( const RefCountedObjectPtr<GameEntity> rhs )
    {
        return hashId == rhs->hashId;
    }

    void highlight( bool on = true );
    void createHighlightMaterial();

    bool isVisible() const;
    void setVisible( bool visible );

    typedef RefCountedObjectPtr<GameEntity> Ptr;
};

class GameEntityManager : public Ogre::Singleton<GameEntityManager>
{
    typedef std::pair<size_t, GameEntity::Ptr> mapping;

    std::map<size_t, GameEntity::Ptr> entities;

    Ogre::SceneManager* sceneManager;
    Ogre::RaySceneQuery* sceneQuery;

public:
    static GameEntityManager* getSingletonPtr();
    static GameEntityManager& getSingleton();

    GameEntityManager();
    ~GameEntityManager();

    void initialise();
    void shutdown();

    bool addGameEntity( GameEntity::Ptr p );
    void removeGameEntity( size_t hashId );
    void removeGameEntity( std::string name );
    GameEntity::Ptr getGameEntity( size_t hashId );
    GameEntity::Ptr getGameEntity( std::string name );

    void update();

    Ogre::Ray getCameraRay( float x, float y );
    std::list<GameEntity::Ptr> mousePick( float x, float y );
    luabridge::LuaRef mousePickLua( lua_State* L, float x, float y );
    luabridge::LuaRef getGameEntityList( lua_State *L );
};

void bindGameEntityClasses( lua_State* L );

#endif // GAMEENTITY_H
