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
#include "gameentity.h"

boost::hash<std::string> GameEntity::hasher;

GameEntity::GameEntity() : hashId(0), sceneNode(0), mesh(0)
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::setName( std::string newName )
{
    GameEntityPtr old;

    // A name change, also changes the hashId. Thus if we are being managed
    // by the GameEntityManager, which uses hashId as a key, we have to
    // remove then re-add ourselves.

    if( hashId )
    {
        // Check if we are being Managed, and remove ourselves if we are.
        // Also get a (the?) shared pointer to ourselves.
        old = GameEntityManager::getSingleton().getGameEntity( hashId );
        GameEntityManager::getSingleton().removeGameEntity( hashId );
    }

    name = newName;
    hashId = hasher( newName );

    if( old )
    {
        // We were being managed.  Re-add ourselves.
        GameEntityManager::getSingleton().addGameEntity( old );
    }
}

void GameEntity::update()
{
}

void GameEntity::collide( GameEntityPtr other )
{
}

template <> GameEntityManager* Ogre::Singleton<GameEntityManager>::ms_Singleton = 0;

GameEntityManager* GameEntityManager::getSingletonPtr()
{
    return ms_Singleton;
}

GameEntityManager& GameEntityManager::getSingleton()
{
    assert( ms_Singleton );
    return *ms_Singleton;
}

GameEntityManager::GameEntityManager() : sceneManager(0), sceneQuery(0)
{
}

GameEntityManager::~GameEntityManager()
{
    assert( !sceneManager );
}

void GameEntityManager::initialise()
{
    using namespace Ogre;

    Root& root = Root::getSingleton();
    sceneManager = root.getSceneManager("SceneManagerInstance");

    assert( sceneManager );

    sceneQuery = sceneManager->createRayQuery(Ray());
    sceneQuery->setSortByDistance(true);
}

// This should be done before the scripting system is shutdown,
// otherwise we are possibly left with shared pointers to Lua objects,
// which when they are free'd will try and access a non-existaint
// Lua state that they have stored a pointer to.
void GameEntityManager::shutdown()
{
    if(sceneManager)
        sceneManager->destroyQuery( sceneQuery );

    sceneManager = 0;

    entities.clear();
}

bool GameEntityManager::addGameEntity( GameEntityPtr p )
{
    assert( p );

    // return false if the hashId is already in the map...
    return entities.insert( mapping( p->hashId, p ) ).second;
}

void GameEntityManager::removeGameEntity( size_t hashId )
{
    entities.erase( hashId );
}

void GameEntityManager::removeGameEntity( std::string name )
{
    size_t hashId = GameEntity::hasher( name );
    entities.erase( hashId );
}

GameEntityPtr GameEntityManager::getGameEntity( size_t hashId )
{
    std::map<size_t, GameEntityPtr>::iterator iter;

    iter = entities.find( hashId );

    if( iter != entities.end() )
        return iter->second;
    else
        return GameEntityPtr();     // Null shared pointer.
}

GameEntityPtr GameEntityManager::getGameEntity( std::string name )
{
    size_t hashId = GameEntity::hasher( name );
    return getGameEntity( hashId );
}

void GameEntityManager::update()
{
    std::map<size_t, GameEntityPtr>::iterator iter;

    for( iter = entities.begin(); iter != entities.end(); iter++ )
        iter->second->update();
}

std::list<GameEntityPtr> GameEntityManager::mousePick( float x, float y )
{
    using namespace Ogre;

    Camera* cam = sceneManager->getCamera("MainCamera");

    Ray ray = cam->getCameraToViewportRay( x, y );

    sceneQuery->setRay( ray );

    RaySceneQueryResult& result = sceneQuery->execute();
    RaySceneQueryResult::iterator iter;

    std::list<GameEntityPtr> picked;
    std::map<size_t, GameEntityPtr>::iterator entityIter;

    for( iter = result.begin(); iter != result.end(); iter++ )
    {
        if( iter->movable )
        {
            // Search for the game entity with this particular mesh instance.
            for( entityIter = entities.begin(); entityIter != entities.end(); entityIter++ )
            {
                if( entityIter->second->mesh == iter->movable )
                {
                    picked.push_back( entityIter->second );
                    break;
                }
            }
        }
    }
    return picked;
}

void GameEntityManager::mousePickLua( lua_State* L, float x, float y )
{
    using namespace Ogre;

    luabind::object geTable = luabind::newtable( L );
    luabind::object distTable = luabind::newtable( L );
    size_t index = 1;

    Camera* cam = sceneManager->getCamera("MainCamera");

    Ray ray = cam->getCameraToViewportRay( x, y );

    sceneQuery->setRay( ray );

    RaySceneQueryResult& result = sceneQuery->execute();
    RaySceneQueryResult::iterator iter;

    std::list<GameEntityPtr> picked;
    std::map<size_t, GameEntityPtr>::iterator entityIter;

    for( iter = result.begin(); iter != result.end(); iter++ )
    {
        if( iter->movable )
        {
            // Search for the game entity with this particular mesh instance.
            for( entityIter = entities.begin(); entityIter != entities.end(); entityIter++ )
            {
                if( entityIter->second->mesh == iter->movable )
                {
                    geTable[index] = entityIter->second;
                    distTable[index] = iter->distance;
                    index++;
                    break;
                }
            }
        }
    }

    // Push return values (tables) onto the Lua stack.

    geTable.push( L );
    distTable.push( L );

    return;
}

void bindGameEntityClasses( lua_State* L )
{
    using namespace luabind;

    module(L)
    [
            class_<GameEntity, GameEntityPtr, GameEntityWrapper>("GameEntity")
            .def( constructor<>() )
            .def( "update", &GameEntity::update, &GameEntityWrapper::default_update )
            .property( "name", &GameEntity::getName, &GameEntity::setName )
            .def_readwrite( "mesh", &GameEntity::mesh )
            .def_readwrite( "node", &GameEntity::sceneNode )
            .property( "hashId", &GameEntity::getHashId )
            .def_readwrite( "parent", &GameEntity::parent )
            .def( "hash", &GameEntity::hash )
            ,
            class_<GameEntityManager>("GameEntityManager")
            .scope
            [
                def( "getSingleton", &GameEntityManager::getSingletonPtr )
            ]
            .def( "add", &GameEntityManager::addGameEntity )
            .def( "remove", (void (GameEntityManager::*)(std::string)) &GameEntityManager::removeGameEntity )
            .def( "remove", (void (GameEntityManager::*)(size_t)) &GameEntityManager::removeGameEntity )
            .def( "get", (GameEntityPtr (GameEntityManager::*)(std::string)) &GameEntityManager::getGameEntity )
            .def( "get", (GameEntityPtr (GameEntityManager::*)(size_t)) &GameEntityManager::getGameEntity )
            .def( "mousePick", &GameEntityManager::mousePickLua )
            //.def( "update", &GameEntityManager::update ) // Usally done every game loop.
    ];
}
