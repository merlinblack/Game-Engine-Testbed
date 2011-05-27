FIND_PACKAGE(Git)
function( enable_git_versioning TARGET )
IF( GIT_EXECUTABLE )
    FILE(WRITE ${CMAKE_BINARY_DIR}/GitVersioning/version.h.in
        "\#define GIT_REPO_VERSION_STR \"@GIT_REPO_VERSION@\"\n"
        )
    FILE(WRITE ${CMAKE_BINARY_DIR}/GitVersioning/version.cmake
    "  EXECUTE_PROCESS(
        COMMAND \"${GIT_EXECUTABLE}\" describe
        RESULT_VARIABLE GIT_RESULT
        OUTPUT_VARIABLE GIT_REPO_VERSION
        ERROR_VARIABLE  GIT_ERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    IF( NOT \${GIT_RESULT} EQUAL 0 )
        EXECUTE_PROCESS(
            COMMAND \"${GIT_EXECUTABLE}\" describe --all
            RESULT_VARIABLE GIT_RESULT
            OUTPUT_VARIABLE GIT_REPO_VERSION
            ERROR_VARIABLE  GIT_ERROR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        IF( NOT \${GIT_RESULT} EQUAL 0 )
            SET(GIT_REPO_VERSION \"unknown version\")
        ENDIF( NOT \${GIT_RESULT} EQUAL 0 )
    ENDIF( NOT \${GIT_RESULT} EQUAL 0 )
    CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
    ")
ELSE( GIT_EXECUTABLE )
    FILE(WRITE ${CMAKE_BINARY_DIR}/GitVersioning/version.h.in
        "\#define GIT_REPO_VERSION_STR \"unknown version\"\n" )
ENDIF( GIT_EXECUTABLE )
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/GitVersioning)
ADD_CUSTOM_TARGET(
    git_versioning
    ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/GitVersioning/version.h.in
                     -D DST=${CMAKE_BINARY_DIR}/GitVersioning/git_versioning.h
                     -P ${CMAKE_BINARY_DIR}/GitVersioning/version.cmake
)
ADD_DEPENDENCIES(${TARGET} git_versioning)
endfunction( enable_git_versioning )
