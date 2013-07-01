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

#include <navigationmesh.h>
#include <algorithm>

//For debugDrawClassification
#include <Ogre.h>

// The speration tolerance that vertices in a navmesh are considered in the same place.
#define VERTEX_SNAP_TOLERANCE 2.0f

NavigationCell::NavigationCell( Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c, int tag )
{
    mVertices[0] = a;
    mVertices[1] = b;
    mVertices[2] = c;
    mLinks[0] = mLinks[1] = mLinks[2] = 0;
    mCentre = ( a + b + c ) / 3;
    mTag = tag;
}

bool NavigationCell::hasVertex( Ogre::Vector3& vec )
{
    if( mVertices[0].positionEquals( vec, VERTEX_SNAP_TOLERANCE ) )   return true;
    if( mVertices[1].positionEquals( vec, VERTEX_SNAP_TOLERANCE ) )   return true;
    if( mVertices[2].positionEquals( vec, VERTEX_SNAP_TOLERANCE ) )   return true;
    return false;
}

void NavigationCell::debugDrawClassification( Ogre::Vector3 start, Ogre::Vector3 end )
{
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* mgr = root->getSceneManager( "SceneManagerInstance" );
    Ogre::ManualObject* debug;
    Ogre::SceneNode* node;

    if( mgr->hasManualObject( "debugDrawClassification" ) )
        debug = mgr->getManualObject( "debugDrawClassification" );
    else
    {
        debug = mgr->createManualObject( "debugDrawClassification" );
        node = mgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject( debug );
        node->translate( 0, 1, 0 );
        debug->setQueryFlags( 0 );
        debug->setRenderQueueGroup( Ogre::RENDER_QUEUE_OVERLAY );
    }

    debug->begin( "debug/blue", Ogre::RenderOperation::OT_LINE_LIST );
    debug->position( start );
    debug->position( end );
    debug->end();

//    debugDrawCell( debug, "debug/yellow", "debug/blue" );
}

void NavigationCell::debugDrawCell( Ogre::ManualObject *debug, Ogre::String matNormal, Ogre::String matSide )
{
    debug->begin( matNormal, Ogre::RenderOperation::OT_LINE_STRIP );
    debug->position( mVertices[0] );
    debug->position( mVertices[1] );
    debug->position( mVertices[2] );
    debug->position( mVertices[0] );
    debug->end();
    if( mLinks[0] == 0 || mLinks[1] == 0 || mLinks[2] == 0 )
    {
        debug->begin( matSide, Ogre::RenderOperation::OT_LINE_LIST );
        if( !mLinks[0] )
        {
            debug->position( mVertices[0] );
            debug->position( mVertices[1] );
        }
        if( !mLinks[1] )
        {
            debug->position( mVertices[1] );
            debug->position( mVertices[2] );
        }
        if( !mLinks[2] )
        {
            debug->position( mVertices[2] );
            debug->position( mVertices[0] );
        }
        debug->end();
    }
}

void NavigationCell::debugDrawCellAndNeigbours()
{
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* mgr = root->getSceneManager( "SceneManagerInstance" );
    Ogre::ManualObject* debug;
    Ogre::SceneNode* debugNode;

    if( mgr->hasSceneNode( "debugDrawNode" ) )
    {
        debugNode = mgr->getSceneNode( "debugDrawNode" );
    }
    else
    {
        debugNode = mgr->getRootSceneNode()->createChildSceneNode( "debugDrawNode" );
        debugNode->translate( 0, 1, 0 );    // Move up slightly to see lines better.
    }

    if( mgr->hasManualObject( "debugDraw" ) )
        debug = mgr->getManualObject( "debugDraw" );
    else
    {
        debug = mgr->createManualObject( "debugDraw" );
        debugNode->attachObject( debug );
        debug->setQueryFlags( 0 );
        debug->setRenderQueueGroup( Ogre::RENDER_QUEUE_OVERLAY );
    }

    for( int i = 0; i < 3; i++ )
    {
        if( mLinks[i] )
        {
            debug->begin( "debug/blue", Ogre::RenderOperation::OT_LINE_STRIP );
            debug->position( mLinks[i]->mVertices[0] );
            debug->position( mLinks[i]->mVertices[1] );
            debug->position( mLinks[i]->mVertices[2] );
            debug->position( mLinks[i]->mVertices[0] );
            debug->end();
        }
    }

    debug->begin( "debug/yellow", Ogre::RenderOperation::OT_LINE_STRIP );
    debug->position( mVertices[0].x, mVertices[0].y+1, mVertices[0].z );
    debug->position( mVertices[1].x, mVertices[1].y+1, mVertices[1].z );
    debug->position( mVertices[2].x, mVertices[2].y+1, mVertices[2].z );
    debug->position( mVertices[0].x, mVertices[0].y+1, mVertices[0].z );
    debug->end();

}

void NavigationCell::getDebugInfoLua( lua_State *L )
{
    /*
    luabind::object info = luabind::newtable(L);
    luabind::object verts = luabind::newtable(L);
    luabind::object links = luabind::newtable(L);

    verts[1] = mVertices[0];
    verts[2] = mVertices[1];
    verts[3] = mVertices[2];

    info["vertices"] = verts;

    links[1] = mLinks[0];
    links[2] = mLinks[1];
    links[3] = mLinks[2];

    info["links"] = links;

    info["g"] = g_cost;
    info["h"] = h_cost;

    info["path"] = path+1;

    info["open"] = isOpen;
    info["closed"] = isClosed;

    info["tag"] = mTag;

    info["centre"] = mCentre;

    info.push( L );
    */

    return;
}

NavigationCell::LINE_CLASSIFICATION NavigationCell::classifyLine2D( Ogre::Vector3& start, Ogre::Vector3& end, NavigationCell* from, NavigationCell*& next )
{
    debugDrawClassification( start, end);

    // Flatten line to 2D
    Ogre::Vector2 s( start.x, start.z );
    Ogre::Vector2 e( end.x, end.z );

    // Flatten triangle co-ordinates.
    Ogre::Vector2 a( mVertices[0].x, mVertices[0].z );
    Ogre::Vector2 b( mVertices[1].x, mVertices[1].z );
    Ogre::Vector2 c( mVertices[2].x, mVertices[2].z );

    // Check line against each side.
    // Note that the value in mLinks may be 0, indicating that there is no cell in that direction.
    // If a line 'exits' a side without a 'neighbour', it has left the confines of the navigation mesh.

    // Side 1
    if( mLinks[0] != from && OgreTools::LinesIntersect2D( s, e, a, b ) )
    {
        next = mLinks[0];
        return LINE_EXITS;
    }
    // Side 2
    if( mLinks[1] != from && OgreTools::LinesIntersect2D( s, e, b, c ) )
    {
        next = mLinks[1];
        return LINE_EXITS;
    }
    // Side 3
    if( mLinks[2] != from && OgreTools::LinesIntersect2D( s, e, c, a ) )
    {
        next = mLinks[2];
        return LINE_EXITS;
    }

    // Line did not cross any sides except the one we share with the cell the line came from.

    if( Ogre::Math::pointInTri2D( e, a, b, c ) )
    {
        next = from;
        return LINE_STOPS;
    }

    next = 0;

    return LINE_MISSED;
}

Ogre::Vector3 NavigationCell::getExitPoint()
{
    // Use the mid point of the cell side we should use to leave this cell.
    switch( path )
    {
        case 0: // Leave via side 0
            return mVertices[0].midPoint( mVertices[1] );

        case 1: // Leave via side 1
            return mVertices[1].midPoint( mVertices[2] );

        case 2: // Leave via side 2
            return mVertices[2].midPoint( mVertices[0] );
    }

    // What? the path should be set if the cell is in the list....
    //assert(0);

    return Ogre::Vector3::ZERO;
}

NavigationMesh::NavigationMesh( Ogre::Vector3 position /* = Ogre::Vector3::ZERO */ ,
                                Ogre::Quaternion rotation /* = Ogre::Quaternion::IDENTITY */ ,
                                Ogre::Vector3 scale /* = Ogre::Vector3::UNIT_SCALE */ ) :
    mPosition( position ),
    mRotation( rotation ),
    mScale( scale ),
    mShow( false )
{
}

NavigationMesh::~NavigationMesh()
{
}

void NavigationMesh::addFromOgreMesh( 
        Ogre::MeshPtr mesh,
        Ogre::Vector3 position,
        Ogre::Quaternion rotation,
        Ogre::Vector3 scale,
        int tag
        )
{
    using namespace Ogre;

    size_t vertex_count;
    Vector3* vertices;
    size_t index_count;
    unsigned long* indices;

    OgreTools::GetMeshInformation( mesh, vertex_count, vertices, index_count, indices,
                                   mPosition + position, 
                                   mRotation * rotation, 
                                   mScale * scale );

    // Add each triangle as a navigation cell.
    for( size_t i = 0; i < index_count; i += 3 )
    {
        NavigationCell cell( vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], tag );

        mCells.push_back( cell );
    }

    delete [] vertices;
    delete [] indices;

    return;
}

void NavigationMesh::computeNeighbours()
{
    NavigationCellList list;
    CellVector::iterator current;
    
    for( current = mCells.begin(); current != mCells.end(); current++ )
        list.push_back( &(*current) );

    computeNeighboursForList( &list );
}

void NavigationMesh::computeNeighboursForList( NavigationCellList *list )
{
    // There will be a better way, but this works for now
    // and this does not run very often.

    NavigationCellList::iterator current;
    CellVector::iterator test;

    for( current = list->begin(); current != list->end(); current++ )
    {
        (*current)->mLinks[0] = 0;
        (*current)->mLinks[1] = 0;
        (*current)->mLinks[2] = 0;

        for( test = mCells.begin(); test != mCells.end(); test++ )
        {
            if( (*current) != &(*test) )
            {
                if( test->hasVertex( (*current)->mVertices[0] ) )
                {
                    if( test->hasVertex( (*current)->mVertices[1] ) )
                    {
                        (*current)->mLinks[0] = &(*test);
                        continue;
                    }
                    if( test->hasVertex( (*current)->mVertices[2] ) )
                    {
                        (*current)->mLinks[2] = &(*test);
                        continue;
                    }
                }
                if( test->hasVertex( (*current)->mVertices[1] ) && test->hasVertex( (*current)->mVertices[2] ) )
                {
                    (*current)->mLinks[1] = &(*test);
                }
            }
        }
    }

    return;
}

void NavigationMesh::addFromOgreEntity( Ogre::Entity *entity,
        Ogre::Vector3 position,
        Ogre::Quaternion rotation,
        Ogre::Vector3 scale,
        int tag
        )
{
    addFromOgreMesh( entity->getMesh(), position, rotation, scale, tag );
}

// Finds cell that contains the specified point, but not necessarily on its surface.
NavigationCell* NavigationMesh::getCellContainingPoint( Ogre::Vector3& p )
{
    using namespace Ogre;

    for( CellVector::iterator i = mCells.begin(); i != mCells.end(); i++ )
    {
        if( Math::pointInTri3D( p, i->mVertices[0], i->mVertices[1], i->mVertices[2],
            Math::calculateBasicFaceNormal( i->mVertices[0], i->mVertices[1], i->mVertices[2] ) ) )
        {
            return &(*i);
        }
    }

    return 0;
}

// Finds cell that contains the specified point, which must be within tolerance of its surface.
NavigationCell* NavigationMesh::getExactCellContainingPoint( Ogre::Vector3& p )
{
    using namespace Ogre;

    Plane plane;

    for( CellVector::iterator i = mCells.begin(); i != mCells.end(); i++ )
    {
        plane.redefine(  i->mVertices[0], i->mVertices[1], i->mVertices[2] );

        // Tolerance found by experiment.
        if( ! Math::RealEqual( plane.getDistance( p ), 0, 1.0f ) )
            continue;   // Point is not on the plane.

        if( Math::pointInTri3D( p, i->mVertices[0], i->mVertices[1], i->mVertices[2],
            Math::calculateBasicFaceNormal( i->mVertices[0], i->mVertices[1], i->mVertices[2] ) ) )
        {
            return &(*i);
        }
    }

    return 0;
}

NavigationPath* NavigationMesh::findNavigationPath( Ogre::Vector3 position, Ogre::Vector3 destination )
{
    NavigationCell* destinationCell = getExactCellContainingPoint( destination );
    if( destinationCell == 0 )
        return 0;   // Destination is not within the navigation mesh.

    NavigationCell* currentCell = getExactCellContainingPoint( position );
    if( currentCell == 0 )
        return 0;   // Current position is not within the navigation mesh.

    NavigationCellList* cellPath = findNavigationCellPath( currentCell, destinationCell );

    //destinationCell->debugDrawCellAndNeigbours();
    //currentCell->debugDrawCellAndNeigbours();

    if( cellPath == 0 )
        return 0;   // No possible path to destination.

    // Create point path from returned cell path.
    NavigationPath* path = new NavigationPath;

    path->push_back( position );

    for( NavigationCellList::iterator i = cellPath->begin(); i != cellPath->end(); i++ )
    {
        // Use the mid point of the cell side we should use to leave this cell.
        path->push_back( (*i)->getExitPoint() );
    }

    path->push_back( destination );

    delete cellPath;

    // Smooth out path? Catmull Rom thingy goes here... ( or called separately on path ).

    return path;
}

void NavigationMesh::findNavigationPathLua( lua_State* L, Ogre::Vector3 position, Ogre::Vector3 destination, Ogre::Radian maxTurnAngle, Ogre::Real pathWidth )
{
    NavigationPath* path = findNavigationPath( position, destination );
    NavigationPath* straightendPath;

    if( path )
    {
        straightendPath = straightenPath( path, maxTurnAngle, pathWidth );
    }
    else
    {
        lua_pushnil( L );
        return;
    }

    // Create a table and populate with the path points.
    /*
    luabind::object table = luabind::newtable( L );

    NavigationPath::iterator i;
    int index = 1;

    for( i = straightendPath->begin(); i != straightendPath->end(); i++ )
    {
        table[index++] = *i;
    }

    delete path;
    delete straightendPath;

    table.push( L );
    */

    return;
}

NavigationPath* NavigationMesh::straightenPath( NavigationPath* path, Ogre::Radian maxTurnAngle, Ogre::Real pathWidth )
{
    NavigationPath* straightenedPath = new NavigationPath;
    NavigationPath::iterator startPoint;
    NavigationPath::iterator endPoint;
    Ogre::Vector3 rightAngleOffset;
    Ogre::Vector3 originalDirection;
    Ogre::Vector3 currentDirection;
    Ogre::Real slope;

    startPoint = path->begin();

    straightenedPath->push_back( *startPoint );

    while( startPoint != path->end() )
    {
        endPoint = startPoint;
        endPoint++;

        originalDirection = *endPoint - *startPoint;
        originalDirection.normalise();
        currentDirection = originalDirection;

        while( endPoint != path->end() )
        {
            slope = currentDirection.y;
            currentDirection = *endPoint - *startPoint;
            currentDirection.normalise();
            rightAngleOffset.x = currentDirection.z;
            rightAngleOffset.y = 0;
            rightAngleOffset.z = -currentDirection.x;
            rightAngleOffset *= pathWidth;

            // Test this line: startPoint -> endPoint

            // Do we turn to much in one spot?
            if( originalDirection.angleBetween( currentDirection ) > maxTurnAngle )
                break;

            // Does the path slope change?
            if( Ogre::Math::Abs(slope - currentDirection.y) > 0.01 )
                break;

            NavigationCell* prevCell;
            NavigationCell* testCell;
            NavigationCell* nextCell;

            Ogre::Vector3 sideStartPoint;
            Ogre::Vector3 sideEndPoint;
            NavigationCell::LINE_CLASSIFICATION ret;

            sideStartPoint = *startPoint + rightAngleOffset;
            sideEndPoint   = *endPoint   + rightAngleOffset;

            prevCell = getExactCellContainingPoint( sideStartPoint );
            if( prevCell )
            {
                ret = prevCell->classifyLine2D( sideStartPoint, sideEndPoint, prevCell, testCell );
                while( ret == NavigationCell::LINE_EXITS && testCell != 0 )
                {
                    ret = testCell->classifyLine2D( sideStartPoint, sideEndPoint, prevCell, nextCell );
                    prevCell = testCell;
                    testCell = nextCell;
                }

                if( testCell == 0 ) // Line leaves the navigation mesh.
                    break;
            }

            sideStartPoint = *startPoint - rightAngleOffset;
            sideEndPoint   = *endPoint   - rightAngleOffset;

            prevCell = getExactCellContainingPoint( sideStartPoint );
            if( prevCell )
            {
                ret = prevCell->classifyLine2D( sideStartPoint, sideEndPoint, prevCell, testCell );
                while( ret == NavigationCell::LINE_EXITS && testCell != 0 )
                {
                    ret = testCell->classifyLine2D( sideStartPoint, sideEndPoint, prevCell, nextCell );
                    prevCell = testCell;
                    testCell = nextCell;
                }

                if( testCell == 0 ) // Line leaves the navigation mesh.
                    break;
            }

            // We survived all the tests.  Move to next the point.
            endPoint++;
        }

        // Either we have got to the end of the points, or a point failed a test.

        if( startPoint >= (endPoint - 1))
        {
            // Point directly after startPoint failed a test... all we can do is just move along.
            startPoint++;
        }
        else
        {
            // We can skip some points.
            startPoint = endPoint - 1;
        }

        if( startPoint != path->end() )
            straightenedPath->push_back( *startPoint );
    }

    return straightenedPath;
}

NavigationCellList* NavigationMesh::findNavigationCellPath( NavigationCell* position, NavigationCell* destination )
{
    NavigationCell* currentCell;
    bool pathFound = false;

    resetPathfinding();

    // Run the A* algorithm from destination to current position.
    pushIntoOpenList( destination );

    destination->g_cost = 0;
    destination->parent = 0;

    while( ! mOpenList.empty() )
    {
        currentCell = popFromOpenList();

        if( currentCell == position )
        {
            // Yahoo, we found our way.
            pathFound = true;
            break;
        }

        // This cell has now been visited.
        currentCell->isClosed = true;

        // For each neighbour of the current cell.
        for( int linkIndex = 0; linkIndex < 3; linkIndex++ )
        {
            NavigationCell* neighbour = currentCell->mLinks[linkIndex];

            if( ! neighbour )   // No neighbor in that direction
                continue;

            if( currentCell->parent == neighbour )  // Kinda pointless
                continue;

            Ogre::Real newcost = currentCell->g_cost + neighbour->mCentre.squaredDistance( currentCell->mCentre );

            if( neighbour->isClosed == false && neighbour->isOpen == false )
            {
                // This is the first time we have been to this cell.
                neighbour->g_cost = newcost;
                neighbour->h_cost = aStarHeuristic( neighbour, position );
                neighbour->totalcost = neighbour->g_cost + neighbour->h_cost;
                neighbour->parent = currentCell;
                pushIntoOpenList(neighbour);
                continue;
            }

            // We've been here before... see if we have a better path now.
            if( newcost < neighbour->g_cost )
            {
                // We have a better path to this cell (neighbour).  Recompute data and
                // either re-add or promote in the open list.
                neighbour->g_cost = newcost;
                // h cost does not change.
                neighbour->totalcost = neighbour->g_cost + neighbour->h_cost;
                neighbour->parent = currentCell;

                // possibly undo closed status
                neighbour->isClosed = false;

                if( neighbour->isOpen == false )
                    pushIntoOpenList( neighbour );
                else
                    promoteCellInOpenList( neighbour );
            }
        }
    }

    if( ! pathFound )
        return 0;

    // Make list.
    // currentCell = position from above code.
    NavigationCellList* path = new NavigationCellList();

    while( currentCell != destination )
    {
        for( int linkIndex = 0; linkIndex < 3; linkIndex++ )
        {
            if( currentCell->mLinks[linkIndex] == currentCell->parent )
            {
                currentCell->path = linkIndex;
                break;
            }
        }
        path->push_back( currentCell );
        currentCell = currentCell->parent;
    }

    return path;
}

Ogre::Real NavigationMesh::aStarHeuristic( NavigationCell* cell, NavigationCell* destination )
{
    // Tweak here!
    return cell->mCentre.squaredDistance( destination->mCentre ) / 256;
}

void NavigationMesh::resetPathfinding()
{
    mOpenList.clear();
    for( CellVector::iterator i = mCells.begin(); i != mCells.end(); i++ )
    {
        i->path = -1;   // Not strictly required.
        i->parent = 0;  // Or this either.

        i->isOpen = false;
        i->isClosed = false;
    }

    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* mgr = root->getSceneManager( "SceneManagerInstance" );
    mgr->destroyManualObject( "debugDrawClassification" );
    mgr->destroyManualObject( "debugDraw" );
}

void NavigationMesh::pushIntoOpenList( NavigationCell* cell )
{
    cell->isOpen = true;
    mOpenList.push_back( cell );
    std::push_heap( mOpenList.begin(), mOpenList.end(), NavigationCellComparison() );
}

NavigationCell* NavigationMesh::popFromOpenList()
{
    NavigationCell* ret = mOpenList.front();

    std::pop_heap( mOpenList.begin(), mOpenList.end(), NavigationCellComparison() );
    mOpenList.pop_back();

    ret->isOpen = false;

    return ret;
}

void NavigationMesh::promoteCellInOpenList( NavigationCell* cell )
{
    for( NavigationCellList::iterator i = mOpenList.begin(); i != mOpenList.end(); i++ )
    {
        if( *i == cell )
        {
            // Reorder heap below and including this cell, to move it up the list.
            std::push_heap( mOpenList.begin(), i + 1, NavigationCellComparison() );
            return;
        }
    }

    // We only get here if the cell was not in the open list.
    assert( 0 );

    return;
}

void NavigationMesh::DebugTextDump( std::ostream &out )
{
    out << "Navigation Mesh" << std::endl;

    for( CellVector::iterator i = mCells.begin(); i != mCells.end(); i++ )
    {
        out << "Cell: " << &(*i) << std::endl;
        out << i->mVertices[0] << std::endl;
        out << i->mVertices[1] << std::endl;
        out << i->mVertices[2] << std::endl;
        out << "Link 1: " << i->mLinks[0] << std::endl;
        out << "Link 2: " << i->mLinks[1] << std::endl;
        out << "Link 3: " << i->mLinks[2] << std::endl;
        out << "Path: " << (i->path + 1) << std::endl;
    }
}

void NavigationMesh::setShow( bool show )
{
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* mgr = root->getSceneManager( "SceneManagerInstance" );
    Ogre::ManualObject* debug;
    Ogre::SceneNode* debugNode;

    mShow = show;

    if( mgr->hasSceneNode( "debugDrawNode2" ) )
    {
        debugNode = mgr->getSceneNode( "debugDrawNode2" );
    }
    else
    {
        debugNode = mgr->getRootSceneNode()->createChildSceneNode( "debugDrawNode2" );
        debugNode->translate( 0, 1, 0 );    // Move up slightly to see lines better.
    }

    if( mgr->hasManualObject( "debugDraw2" ) )
        debug = mgr->getManualObject( "debugDraw2" );
    else
    {
        debug = mgr->createManualObject( "debugDraw2" );
        debugNode->attachObject( debug );
        debug->setQueryFlags( 0 );
        debug->setRenderQueueGroup( Ogre::RENDER_QUEUE_OVERLAY );
    }

    if( !mShow )
    {
        mgr->destroyManualObject( debug );
        return;
    }

    for( CellVector::iterator i = mCells.begin(); i != mCells.end(); i++ )
    {
        i->debugDrawCell( debug, "debug/yellow", "debug/blue" );
    }

    return;
}
