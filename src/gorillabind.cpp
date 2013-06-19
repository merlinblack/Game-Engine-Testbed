#include <lua.hpp>
#include <LuaBridge.h>
#include <Gorilla.h>
#include <ward_ptr.h>

class Test
{
    Ogre::String text;

    public:
    void setText( Ogre::String newString )
    {
        text = newString;
    }
    Ogre::String getText()
    {
        return text;
    }
};

namespace luabridge
{
    template <class T>
        struct ContainerTraits <ward_ptr <T> >
        {
            typedef T Type;

            static T* get (ward_ptr <T> const& c)
            {
                return c.get();
            }
        };
};

ward_ptr<Test> getNewTest( Ogre::String text )
{
    Test *t = new Test();

    t->setText( text );

    return ward_ptr<Test>(t);
}

void zapTest( ward_ptr<Test> ptr )
{
    delete ptr.get();

    ptr.invalidate();
}

void bindGorilla( lua_State* L )
{
    using namespace luabridge;
    using namespace Gorilla;

    getGlobalNamespace( L )
        .beginNamespace( "Gorilla" )
        .beginClass<Silverback>( "Silverback" )
        .endClass()
        .beginClass<Test>( "Test" )
        .addStaticFunction( "create",  &getNewTest )
        .addStaticFunction( "destroy", &zapTest )
        .addFunction( "getText", &Test::getText )
        .addFunction( "setText", &Test::setText )
        .endClass()
        .endNamespace();

}
