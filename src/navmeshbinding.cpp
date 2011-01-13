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
#include <luabind/luabind.hpp>

#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

using namespace luabind;

void bindNavigationMesh( lua_State* L )
{
    module(L)
    [
        class_< NavigationCell >( "NavigationCell" )
        .def( "info", &NavigationCell::getDebugInfoLua )
        .def( "draw", &NavigationCell::debugDrawCellAndNeigbours ),
        class_< NavigationMesh, NavigationMeshPtr >( "NavigationMesh" )
        .def( constructor< Ogre::Vector3, Ogre::Quaternion, Ogre::Vector3 >() )
        .def( constructor< Ogre::Vector3, Ogre::Quaternion >() )
        .def( constructor< Ogre::Vector3 >() )
        .def( constructor<>() )
        .def( "addFromEntity", &NavigationMesh::addFromOgreEntity )
        .def( "addFromEntity", (void (NavigationMesh::*)
                    ( Ogre::Entity*, Ogre::Vector3, Ogre::Quaternion ))&NavigationMesh::addFromOgreEntity )
        .def( "addFromEntity", (void (NavigationMesh::*)
                    ( Ogre::Entity*, Ogre::Vector3 ))&NavigationMesh::addFromOgreEntity )
        .def( "addFromEntity", (void (NavigationMesh::*)
                    ( Ogre::Entity* ))&NavigationMesh::addFromOgreEntity )
        .def( "computeNeighbours", &NavigationMesh::computeNeighbours )
        .def( "findPath", &NavigationMesh::findNavigationPathLua )
        .property( "show", &NavigationMesh::getShow, &NavigationMesh::setShow )
        .def( "getCellAtPoint", &NavigationMesh::getExactCellContainingPoint )
    ];
}
