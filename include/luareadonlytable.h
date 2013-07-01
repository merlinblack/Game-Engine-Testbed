#include <lua.hpp>
#include <LuaBridge.h>

// Creates a proxy table that allows only reading the values to the input table.
// The input table is not altered, and any reference to it sill has read and write
// access.
// The proxy table supports iteration via pairs()
//
luabridge::LuaRef makeReadonlyProxy( luabridge::LuaRef table );
