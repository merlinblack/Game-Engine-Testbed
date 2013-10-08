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

#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreQuaternion.h>

using namespace luabridge;

void bindAnimation( lua_State* L )
{
    getGlobalNamespace(L)
        .beginNamespace("Engine")
            .beginClass< AnimationManager >( "AnimationManager" )
            .addStaticFunction( "getSingleton", &AnimationManager::getSingleton )
            .addFunction( "add", &AnimationManager::addAnimation )
            .addFunction( "remove", &AnimationManager::removeAnimation )
            .endClass()

            .beginClass< ::Animation >( "Animation" )
            .addFunction( "stop", &::Animation::stop )
            .addFunction( "start", &::Animation::start )
            .addFunction( "isFinished", &::Animation::isFinished )
            //.addFunction( self == other< AnimationPtr >() ) // TODO add equality test??
            .endClass()

            .deriveClass< MeshAnimation, ::Animation >( "MeshAnimation" )
            .addConstructor< void (*) (Ogre::Entity*, Ogre::String) >()
            .addFunction( "fadeIn", &MeshAnimation::fadeIn )
            .addFunction( "fadeOut", &MeshAnimation::fadeOut )
            .addFunction( "setWeight", &MeshAnimation::setWeighting )
            .addFunction( "setFadeSpeed", &MeshAnimation::setFadeSpeed )
            .addFunction( "setLoop", &MeshAnimation::setLoop )
            .addFunction( "isFadingIn", &MeshAnimation::isFadingIn )
            .addFunction( "isFadingOut", &MeshAnimation::isFadingOut )
            .endClass()

            .deriveClass< MovementAnimation, ::Animation >( "MovementAnimation" )
            .addConstructor< void (*) ( Ogre::SceneNode*, Ogre::Vector3, Ogre::Real ) >()
            .endClass()

            .deriveClass< RotationAnimation, ::Animation >( "RotationAnimation" )
            .addConstructor< void (*) ( Ogre::SceneNode*, Ogre::Quaternion, Ogre::Real ) >()
            .endClass()
            ;
}

