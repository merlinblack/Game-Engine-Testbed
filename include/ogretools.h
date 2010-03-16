#ifndef __OGRETOOLS_H
#define __OGRETOOLS_H

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace OgreTools
{
    bool LinesIntersect2D( Ogre::Vector2 firstStart, Ogre::Vector2 firstEnd,
                              Ogre::Vector2 secondStart, Ogre::Vector2 secondEnd );

    void GetMeshInformation(const Ogre::MeshPtr mesh,
            size_t &vertex_count,
            Ogre::Vector3* &vertices,
            size_t &index_count,
            unsigned long* &indices,
            const Ogre::Vector3 &position,
            const Ogre::Quaternion &orient,
            const Ogre::Vector3 &scale);
}

#endif // __OGRETOOLS_H
