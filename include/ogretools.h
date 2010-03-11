#ifndef __OGRETOOLS_H
#define __OGRETOOLS_H

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace OgreTools
{
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
