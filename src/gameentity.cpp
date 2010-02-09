#include "gameentity.h"

boost::hash<std::string> GameEntity::hasher;

GameEntity::GameEntity() : hashId(0), mesh(0), sceneNode(0)
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
            //.def( "update", &GameEntityManager::update ) // Usally done every game loop.
    ];
}
