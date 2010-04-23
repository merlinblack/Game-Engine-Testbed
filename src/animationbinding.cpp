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

#include <animation.h>
#include <luabind/luabind.hpp>

#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreQuaternion.h>

using namespace luabind;

void bindAnimation( lua_State* L )
{
    module(L)
    [
            class_< AnimationManager >( "AnimationManager" )
            .scope
            [
                    def( "getSingleton", &AnimationManager::getSingleton )
            ]
            .def( "add", &AnimationManager::addAnimation )
            .def( "remove", &AnimationManager::removeAnimation )
    ];

    module(L)
    [
            class_< Animation, AnimationPtr >( "Animation" )
            .def( "stop", &Animation::stop )
            .def( "start", &Animation::start )
            .def( "isFinished", &Animation::isFinished )
    ];

    module(L)
    [
            class_< MeshAnimation, Animation, AnimationPtr >( "MeshAnimation" )
            .def( constructor< Ogre::Entity*, Ogre::String >())
            //.def( "start", &MeshAnimation::start )
            //.def( "stop", &MeshAnimation::stop )
            .def( "fadeIn", &MeshAnimation::fadeIn )
            .def( "fadeOut", &MeshAnimation::fadeOut )
            .def( "setWeight", &MeshAnimation::setWeighting )
            .def( "setFadeSpeed", &MeshAnimation::setFadeSpeed )
            .def( "setLoop", &MeshAnimation::setLoop )
            .def( "isFadingIn", &MeshAnimation::isFadingIn )
            .def( "isFadingOut", &MeshAnimation::isFadingOut )
    ];

    module(L)
    [
            class_< MovementAnimation, Animation, AnimationPtr >( "MovmentAnimation" )
            .def( constructor< Ogre::SceneNode*, Ogre::Vector3, Ogre::Real >())
            //.def( "start", &MeshAnimation::start )
            //.def( "stop", &MeshAnimation::stop )
    ];

    module(L)
    [
            class_< RotationAnimation, Animation, AnimationPtr >( "RotationAnimation" )
            .def( constructor< Ogre::SceneNode*, Ogre::Quaternion, Ogre::Real >())
            //.def( "start", &MeshAnimation::start )
            //.def( "stop", &MeshAnimation::stop )
    ];
}

