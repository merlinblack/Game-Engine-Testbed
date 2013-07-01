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
#include <ogretools.h>
#include <ostream>

std::ostream& operator<<( std::ostream& stream, const GameEntity& ge );
std::ostream& operator<<( std::ostream& stream, const GameEntityManager& gm );

boost::hash<std::string> GameEntity::hasher;

GameEntity::GameEntity() : hashId(0), sceneNode(0), mesh(0), originalMaterial(0), highlightMaterial(0)
{
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
    // Nothing, might get overridden in Lua
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

bool GameEntity::isVisible()
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
    sceneManager = root.getSceneManager("SceneManagerInstance");

    assert( sceneManager );

    sceneQuery = sceneManager->createRayQuery(Ray());
    sceneQuery->setSortByDistance(true);
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

Ogre::Ray GameEntityManager::getCameraRay( float x, float y )
{
    Ogre::Camera* cam = sceneManager->getCamera("MainCamera");

    return cam->getCameraToViewportRay( x, y );
}

std::list<GameEntityPtr> GameEntityManager::mousePick( float x, float y )
{
    using namespace Ogre;

    sceneQuery->setRay( getCameraRay( x, y ) );

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
    /*
    using namespace Ogre;

    //TODO convert to LuaBridge
    luabind::object geTable = luabind::newtable( L );
    luabind::object distTable = luabind::newtable( L );
    size_t index = 1;

    sceneQuery->setRay( getCameraRay( x, y ) );

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
*/
    return;
}

void GameEntityManager::getGameEntityList( lua_State *L )
{
    /*
    luabind::object geTable = luabind::newtable( L );
    size_t index = 0;

    std::map<size_t, GameEntityPtr>::iterator entityIter;

    for( entityIter = entities.begin(); entityIter != entities.end(); entityIter++ )
    {
        geTable[++index] = entityIter->second;
    }

    geTable.push( L );
    */
}

std::ostream& operator<<( std::ostream& stream, const GameEntity& ge )
{
    return stream << "Game Entity: " << ge.getName();
}

std::ostream& operator<<( std::ostream& stream, const GameEntityManager& gm )
{
    return stream << "Game Entity Manager (Singleton)";
}

void bindGameEntityClasses( lua_State* L )
{
    /*
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
            .def( "hitCheck", &GameEntity::hitCheck )
            .def( "hitPosition", &GameEntity::hitPosition )
            .def( "highlight", &GameEntity::highlight )
            .def( "createHighlightMaterial", &GameEntity::createHighlightMaterial )
            .def( "removeFromManager", &GameEntity::removeFromManager )
            .property( "visible", &GameEntity::isVisible, &GameEntity::setVisible )
            .def(tostring(self))
            .def( self == other<GameEntityPtr>() )
            .def("__finalize", &GameEntity::removeFromManager)
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
            .def( "getEntityList", &GameEntityManager::getGameEntityList )
            .def(tostring(self))
            //.def( "update", &GameEntityManager::update ) // Usually done every game loop.
    ];
    */
}
