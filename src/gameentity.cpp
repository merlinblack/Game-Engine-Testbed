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
#include <gameentity.h>
#include <OgreRoot.h>
#include <OgreMath.h>
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>
#include <ogretools.h>

#include <iostream>

using std::cout;
using std::endl;

boost::hash<std::string> GameEntity::hasher;

GameEntity::GameEntity( lua_State* L ) : luaData(L), update_overide(L), hashId(0), sceneNode(0), mesh(0), originalMaterial(0), highlightMaterial(0)
{
    luaData = luabridge::newTable( L );
}

GameEntity::~GameEntity()
{
    if( sceneNode )
    {
        sceneNode->getCreator()->destroySceneNode( sceneNode );
    }
}

void GameEntity::setName( std::string newName )
{
    GameEntity::Ptr old;

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
    if( update_overide.isFunction() )
        update_overide();
}

void GameEntity::removeFromManager()
{
    GameEntityManager::getSingleton().removeGameEntity( hashId );
}

// Checks if a ray from camera at (x,y) intersects with *any* mesh triangles.
// Returns as soon as an intersection is found.
bool GameEntity::hitCheck( float x, float y )
{
    // Get mesh data and test each triangle for intersection.
    // once we find one, we're out.
    size_t vertex_count;
    Ogre::Vector3* vertices;
    size_t index_count;
    unsigned long* indices;

    OgreTools::GetMeshInformation( mesh->getMesh(), vertex_count, vertices, index_count, indices,
            sceneNode->_getDerivedPosition(),
            sceneNode->_getDerivedOrientation(),
            sceneNode->getScale() );

    Ogre::Ray ray = GameEntityManager::getSingleton().getCameraRay( x, y );

    bool foundHit = false;

    for( size_t i = 0; i < index_count; i += 3 )
    {
        std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects( ray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false );

        if( hit.first )
        {
            foundHit = true;
            break;
        }
    }

    delete [] vertices;
    delete [] indices;

    return foundHit;
}

// Returns the position of intersection with the mesh triangle that results in
// the smallest distance from the camera.
Ogre::Vector3 GameEntity::hitPosition( float x, float y )
{
    // Get mesh data and test each triangle for intersection.

    size_t vertex_count;
    Ogre::Vector3* vertices;
    size_t index_count;
    unsigned long* indices;

    OgreTools::GetMeshInformation( mesh->getMesh(), vertex_count, vertices, index_count, indices,
            sceneNode->_getDerivedPosition(),
            sceneNode->_getDerivedOrientation(),
            sceneNode->getScale() );

    Ogre::Ray ray = GameEntityManager::getSingleton().getCameraRay( x, y );

    Ogre::Real distance = Ogre::Math::POS_INFINITY;

    for( size_t i = 0; i < index_count; i += 3 )
    {
        std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects( ray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false );

        if( hit.first )
        {
            if( hit.second < distance )
                distance = hit.second;
        }
    }

    delete [] vertices;
    delete [] indices;

    if( distance == Ogre::Math::POS_INFINITY )
        return Ogre::Vector3( Ogre::Math::POS_INFINITY );

    return ray.getPoint( distance );
}

void GameEntity::highlight( bool on /* = true */ )
{
    if( highlightMaterial.isNull() )
        createHighlightMaterial();

    if( on )
        mesh->setMaterial( highlightMaterial );
    else
        mesh->setMaterial( originalMaterial );
}

void GameEntity::createHighlightMaterial()
{
    originalMaterial = mesh->getSubEntity( 0 )->getMaterial();
    Ogre::String name = originalMaterial->getName() + "_highLight";

    if( Ogre::MaterialManager::getSingleton().resourceExists( name ) )
    {
        highlightMaterial = Ogre::MaterialManager::getSingleton().getByName( name );
    }
    else
    {
        highlightMaterial = originalMaterial->clone( name );

        Ogre::Technique* technique = highlightMaterial->getBestTechnique();

        Ogre::ColourValue colour( 1.0f, 1.0f, 0.0f, 0.7f );

        Ogre::Pass* pass = technique->createPass();
        pass->setDepthWriteEnabled( false );
        pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
        pass->setAmbient( colour );
        pass->setDiffuse( colour );
        pass->setSelfIllumination( colour );
        pass->setShininess( 64.0f );
        pass->setSpecular( Ogre::ColourValue::White );
    }
}

bool GameEntity::isVisible() const
{
    if( ! mesh )
        return false;

    return mesh->isVisible();
}

void GameEntity::setVisible( bool visible )
{
    if( ! mesh )
        return;

    mesh->setVisible( visible );
}

#if OGRE_VERSION < 67584 /* 1.8.0 */
template <> GameEntityManager* Ogre::Singleton<GameEntityManager>::ms_Singleton = 0;
#else
template <> GameEntityManager* Ogre::Singleton<GameEntityManager>::msSingleton = 0;
#endif

GameEntityManager* GameEntityManager::getSingletonPtr()
{
#if OGRE_VERSION < 67584 /* 1.8.0 */
    return ms_Singleton;
#else
    return msSingleton;
#endif
}

GameEntityManager& GameEntityManager::getSingleton()
{
#if OGRE_VERSION < 67584 /* 1.8.0 */
    assert( ms_Singleton );
    return *ms_Singleton;
#else
    assert( msSingleton );
    return *msSingleton;
#endif
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
    try
    {
        sceneManager = root.getSceneManager("SceneManagerInstance");
        assert( sceneManager );

        sceneQuery = sceneManager->createRayQuery(Ray());
        sceneQuery->setSortByDistance(true);
    }
    catch( ... )
    {
    }
}

// This should be done before the scripting system is shutdown,
// otherwise we are possibly left with shared pointers to Lua objects,
// which when they are freed will try and access a non-existing
// Lua state that they have stored a pointer to.
void GameEntityManager::shutdown()
{
    if(sceneManager)
        sceneManager->destroyQuery( sceneQuery );

    sceneManager = 0;

    entities.clear();
}

bool GameEntityManager::addGameEntity( GameEntity::Ptr p )
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

GameEntity::Ptr GameEntityManager::getGameEntity( size_t hashId )
{
    std::map<size_t, GameEntity::Ptr>::iterator iter;

    iter = entities.find( hashId );

    if( iter != entities.end() )
        return iter->second;
    else
        return GameEntity::Ptr();     // TODO: Fix this!!! Was a Null shared pointer.
}

GameEntity::Ptr GameEntityManager::getGameEntity( std::string name )
{
    size_t hashId = GameEntity::hasher( name );
    return getGameEntity( hashId );
}

void GameEntityManager::update()
{
    std::map<size_t, GameEntity::Ptr>::iterator iter;

    for( iter = entities.begin(); iter != entities.end(); iter++ )
        iter->second->update();
}

Ogre::Ray GameEntityManager::getCameraRay( float x, float y )
{
    assert( sceneManager );

    Ogre::Camera* cam = sceneManager->getCamera("MainCamera");

    return cam->getCameraToViewportRay( x, y );
}

std::list<GameEntity::Ptr> GameEntityManager::mousePick( float x, float y )
{
    using namespace Ogre;

    sceneQuery->setRay( getCameraRay( x, y ) );

    RaySceneQueryResult& result = sceneQuery->execute();
    RaySceneQueryResult::iterator iter;

    std::list<GameEntity::Ptr> picked;
    std::map<size_t, GameEntity::Ptr>::iterator entityIter;

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

luabridge::LuaRef GameEntityManager::mousePickLua( float x, float y, lua_State* L )
{
    using namespace Ogre;
    using namespace luabridge;

    LuaRef geTable = newTable( L );
    LuaRef distTable = newTable( L );

    sceneQuery->setRay( getCameraRay( x, y ) );

    RaySceneQueryResult& result = sceneQuery->execute();
    RaySceneQueryResult::iterator iter;

    std::map<size_t, GameEntity::Ptr>::iterator entityIter;

    for( iter = result.begin(); iter != result.end(); iter++ )
    {
        if( iter->movable )
        {
            // Search for the game entity with this particular mesh instance.
            for( entityIter = entities.begin(); entityIter != entities.end(); entityIter++ )
            {
                if( entityIter->second->mesh == iter->movable )
                {
                    geTable.append( entityIter->second );
                    distTable.append( iter->distance );
                    break;
                }
            }
        }
    }

    LuaRef retTable = newTable( L );

    retTable["entities"] = geTable;
    retTable["distances"] = distTable;

    return retTable;
}

luabridge::LuaRef GameEntityManager::getGameEntityList( lua_State *L )
{
    using namespace luabridge;

    LuaRef geTable = newTable( L );

    std::map<size_t, GameEntity::Ptr>::iterator entityIter;

    for( entityIter = entities.begin(); entityIter != entities.end(); entityIter++ )
    {
        geTable.append( entityIter->second );
    }

    return geTable;
}

Ogre::String GameEntityToString( const GameEntity::Ptr ge )
{
    return "Game Entity: " + ge->getName() + " " + Ogre::StringConverter::toString(ge->getHashId(), 0, ' ', std::ios::hex );
}

Ogre::String GameEntityManagerToString( const GameEntityManager* gm )
{
    return "Game Entity Manager (Singleton)";
}

void bindGameEntityClasses( lua_State* L )
{
    using namespace luabridge;

    getGlobalNamespace( L )
        .beginNamespace( "Engine" )
        .beginClass<GameEntity>( "GameEntity" )
        .addConstructor<void (*)(lua_State*), RefCountedObjectPtr<GameEntity> >()
        .addData( "data", &GameEntity::luaData )
        .addData( "updateOveride", &GameEntity::update_overide )
        .addData( "walkable", &GameEntity::walkable )
        .addFunction( "update", &GameEntity::update )
        .addProperty( "name", &GameEntity::getName, &GameEntity::setName )
        .addData( "mesh", &GameEntity::mesh )
        .addData( "node", &GameEntity::sceneNode )
        .addFunction( "hashId", &GameEntity::getHashId )
        .addData( "parent", &GameEntity::parent )
        .addFunction( "hash", &GameEntity::hash )
        .addFunction( "hitCheck", &GameEntity::hitCheck )
        .addFunction( "hitPosition", &GameEntity::hitPosition )
        .addFunction( "highlight", &GameEntity::highlight )
        .addFunction( "createHighlightMaterial", &GameEntity::createHighlightMaterial )
        .addFunction( "removeFromManager", &GameEntity::removeFromManager )
        .addProperty( "visible", &GameEntity::isVisible, &GameEntity::setVisible )
        .addFunction( "__tostring", &GameEntityToString )
        .endClass()
        .beginClass<GameEntityManager>("GameEntityManager")
        .addStaticFunction( "getSingleton", &GameEntityManager::getSingletonPtr )
        .addFunction( "add", &GameEntityManager::addGameEntity )
        .addFunction( "remove", (void (GameEntityManager::*)(std::string)) &GameEntityManager::removeGameEntity )
        .addFunction( "removeById", (void (GameEntityManager::*)(size_t)) &GameEntityManager::removeGameEntity )
        .addFunction( "get", (GameEntity::Ptr (GameEntityManager::*)(std::string)) &GameEntityManager::getGameEntity )
        .addFunction( "getById", (GameEntity::Ptr (GameEntityManager::*)(size_t)) &GameEntityManager::getGameEntity )
        .addFunction( "mousePick", &GameEntityManager::mousePickLua )
        .addFunction( "getEntityList", &GameEntityManager::getGameEntityList )
        .addFunction( "__tostring", &GameEntityManagerToString )
        .endClass()
        .endNamespace();
}
