#ifndef NAVIGATIONMESH_H
#define NAVIGATIONMESH_H

#include <ogretools.h>
#include <OgreVector3.h>
#include <OgreMesh.h>

#include <vector>
#include <ostream>

class NavigationMesh;

class NavigationCell
{
    friend class NavigationMesh;

    Ogre::Vector3 mVertices[3];
    NavigationCell* mLinks[3];

    int SerialisationID;

    bool hasVertex( Ogre::Vector3& vec );

public:
    NavigationCell( Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c );
};

class NavigationMesh
{
    typedef std::vector<NavigationCell> CellVector;
    CellVector mCells;

    Ogre::Vector3 mPosition;
    Ogre::Quaternion mRotation;
    Ogre::Vector3 mScale;

public:
    NavigationMesh( Ogre::Vector3 position = Ogre::Vector3::ZERO,
                    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY,
                    Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE );
    ~NavigationMesh();

    void BuildFromOgreMesh( Ogre::MeshPtr mesh );

    void DebugTextDump( std::ostream &out );
};

#endif // NAVIGATIONMESH_H
