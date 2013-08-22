set (Lua_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE )
set (${PROJECT_NAME}_LUA_C_FLAGS "-O3" CACHE STRING "${PROJECT_NAME} Extra C Flags for Lua" )

# Extra C flags for Lua
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${${PROJECT_NAME}_LUA_C_FLAGS}")
# Remove debug if it is there.
string(REPLACE "-g" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})




if(WIN32)
    set (Lua_LIBRARIES lua PARENT_SCOPE )
    set (Lua_LIBRARIES lua )
elseif(UNIX)
    set(Lua_LIBRARIES lua dl m PARENT_SCOPE )
    set(Lua_LIBRARIES lua dl m )
    set(Lua_EXEC_LIBRARIES readline ncurses)
    add_definitions(-DLUA_USE_LINUX)
endif()

# define the lua core source files
set (Lua_CORE_SRCS lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c
     lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c
     lparser.c lstate.c lstring.c ltable.c ltm.c  lundump.c
     lvm.c lzio.c)

# define the lua lib source files
set (Lua_LIB_SRCS lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c
     liolib.c lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c
     linit.c)

# create the library
add_library (lua ${Lua_LIB_SRCS} ${Lua_CORE_SRCS})

# create the lua executable and link it to the lib
add_executable (luaexec lua.c)
target_link_libraries (luaexec ${Lua_LIBRARIES} ${Lua_EXEC_LIBRARIES})

# name the executable lua just like the library
set_target_properties(luaexec PROPERTIES OUTPUT_NAME lua)

# create the luac
add_executable (luac luac.c)
target_link_libraries (luac ${Lua_LIBRARIES} ${Lua_EXEC_LIBRARIES})
