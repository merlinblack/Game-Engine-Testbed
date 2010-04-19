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

#ifndef NAVIGATIONMESH_H
#define NAVIGATIONMESH_H

/*
Implemenation Notes:

Based roughly on "Simplfied 3D Movement and Pathfinding Using Navigation Meshes" from
Game Programming Gems 1 - Page 288.
The mesh cells are triangles and assumed to have normals no more than 90 degrees from
the up/down Y axis.  This allows us to treat it as a flat 2D structure at times.

Path finding from cell to cell uses A*.

Ogre::Vector3.squaredDistance is used rather than Ogre::Vector3.distance for costs as
this avoids calling sqrt, and the costs are only ever compared against each other.
*/


#include <ogretools.h>
#include <OgreVector3.h>
#include <OgreMesh.h>

#include <vector>
#include <ostream>

class NavigationMesh;
struct NavigationCellComparison;

class NavigationCell
{
    friend class NavigationMesh;
    friend class NavigationCellComparison;

    Ogre::Vector3 mVertices[3];
    Ogre::Vector3 mCentre;

    // Pointers to cell's neighbours.
    NavigationCell* mLinks[3];

    // *** Pathfinding
    int path;               // Which neighbour is next in the found path.
    bool isOpen;
    bool isClosed;
    Ogre::Real g_cost;      // cost to get here.
    Ogre::Real h_cost;      // guess at cost to destination
    Ogre::Real totalcost;
    NavigationCell* parent; // cell that we got here from.
    // ***

    bool hasVertex( Ogre::Vector3& vec );
    void debugDrawClassification( Ogre::Vector3 start, Ogre::Vector3 end );

public:
    NavigationCell( Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c );

    enum LINE_CLASSIFICATION
    {
        LINE_EXITS,
        LINE_STOPS,
        LINE_MISSED
    };

    // Y component of the line and cell's triangle is ignored
    LINE_CLASSIFICATION classifyLine2D( Ogre::Vector3& start, Ogre::Vector3& end,
                                        NavigationCell* from, NavigationCell* &next );

    Ogre::Vector3 getExitPoint();
};

struct NavigationCellComparison
{
    bool operator() ( NavigationCell* first, NavigationCell* second )
    {
        return first->totalcost > second->totalcost;
    }
};

typedef std::vector<Ogre::Vector3> NavigationPath;
typedef std::vector<NavigationCell*> NavigationCellList;

class NavigationMesh
{
    typedef std::vector<NavigationCell> CellVector;
    CellVector mCells;

    Ogre::Vector3 mPosition;
    Ogre::Quaternion mRotation;
    Ogre::Vector3 mScale;

    // This is used as a binary heap.
    NavigationCellList mOpenList;

public:
    NavigationMesh( Ogre::Vector3 position = Ogre::Vector3::ZERO,
                    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY,
                    Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE );
    ~NavigationMesh();

    void BuildFromOgreMesh( Ogre::MeshPtr mesh );

    // Finds cell that contains the specified point, but not necessarily on its surface.
    NavigationCell* getCellContainingPoint( Ogre::Vector3& p );
    // Finds cell that contains the specified point, which must be within tolerance of its surface.
    NavigationCell* getExactCellContainingPoint( Ogre::Vector3& p );

    NavigationPath* findNavigationPath( Ogre::Vector3 position, Ogre::Vector3 destination );
    NavigationCellList* findNavigationCellPath( NavigationCell* position, NavigationCell* destination );

    NavigationPath* straightenPath( NavigationPath* path, Ogre::Radian maxTurnAngle, Ogre::Real width );

    void DebugTextDump( std::ostream &out );

private:
    inline Ogre::Real aStarHeuristic( NavigationCell* cell, NavigationCell* destination );
    void resetPathfinding();
    inline void pushIntoOpenList( NavigationCell* cell );
    inline NavigationCell* popFromOpenList();
    inline void promoteCellInOpenList( NavigationCell* cell );
};

#endif // NAVIGATIONMESH_H
