#include <lua.hpp>
#include <LuaBridge.h>
#include <Gorilla.h>

void bindGorilla( lua_State* L )
{
    using namespace luabridge;
    using namespace Gorilla;

    getGlobalNamespace( L )
        .beginNamespace( "Gorilla" )
            .beginClass<Silverback>( "Silverback" )
            .endClass()
        .endNamespace();

}
