cmake_minimum_required(VERSION 2.6)
project (engine)

if (WIN32)
    set(CMAKE_MODULE_PATH_ADD $ENV{PROGRAMFILES}/OGRE/CMake $ENV{OGRE_HOME}/CMake )
elseif (UNIX)
    set(CMAKE_MODULE_PATH_ADD /usr/lib/OGRE/cmake /usr/lib64/OGRE/cmake /usr/local/lib/OGRE/cmake )
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} " " ${CMAKE_MODULE_PATH_ADD} " " ${PROJECT_SOURCE_DIR}/cmake )
MESSAGE(STATUS "Searching in: ${CMAKE_MODULE_PATH} ")

set(OGRE_STATIC false)

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OGRE)
FIND_PACKAGE(OIS)
SET(Boost_USE_STATIC_LIBS    OFF)
SET(Boost_USE_MULTITHREADED  ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.40.0 COMPONENTS thread date_time)
FIND_PACKAGE(Threads)

add_subdirectory( Lua )

# For LuaBridge
if(UNIX)
  SET(LuaBridge_FLAGS "-Wno-deprecated")
endif()

INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/LuaBridge
	${OpenGL_INCLUDE_DIR}
	${OGRE_INCLUDE_DIRS}
	${OIS_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
    ${Lua_INCLUDE_DIRS}
	)

FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HDRS include/*.h)
FILE(GLOB SCRIPTS scripts/*.lua scripts/gui/*.lua)

add_definitions (-DGORILLA_USES_EXCEPTIONS=1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LuaBridge_FLAGS}" )

ADD_EXECUTABLE(engine
	${SRCS}
	${HDRS}
    ${SCRIPTS}
	)

TARGET_LINK_LIBRARIES(engine
	${OIS_LIBRARIES}
	${OGRE_LIBRARIES}
	${Boost_LIBRARIES}
    ${Lua_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
	)

INCLUDE( GitVersioning )
enable_git_versioning( engine )

MESSAGE(STATUS "Boost libraries:    ${Boost_LIBRARIES} ")
MESSAGE(STATUS "OIS libraries:      ${OIS_LIBRARIES} ")
MESSAGE(STATUS "OIS headers:        ${OIS_INCLUDE_DIRS} ")
MESSAGE(STATUS "OGRE libraries:     ${OGRE_LIBRARIES} ")
MESSAGE(STATUS "Lua libraries:      ${Lua_LIBRARIES} ")
MESSAGE(STATUS "Lua headers:        ${Lua_INCLUDE_DIRS} ")
