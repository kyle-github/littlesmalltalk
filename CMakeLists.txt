cmake_minimum_required(VERSION 3.0)

set( PROJECT_NAME "LittleSmalltalk" )

project(${PROJECT_NAME})

set (VERSION_MAJOR 4)
set (VERSION_MINOR 7)
set (VERSION_PATCH 1)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# make the version file.
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/src/vm/version.h.in" "${PROJECT_SOURCE_DIR}/src/vm/version.h" @ONLY)

set(COMMON_SRCS 
            "${PROJECT_SOURCE_DIR}/src/vm/err.c"
            "${PROJECT_SOURCE_DIR}/src/vm/err.h"
            "${PROJECT_SOURCE_DIR}/src/vm/globals.c"
            "${PROJECT_SOURCE_DIR}/src/vm/globals.h"
            "${PROJECT_SOURCE_DIR}/src/vm/image.c"
            "${PROJECT_SOURCE_DIR}/src/vm/image.h"
            "${PROJECT_SOURCE_DIR}/src/vm/interp.c"
            "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
            "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
            "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
            "${PROJECT_SOURCE_DIR}/src/vm/prim.c"
            "${PROJECT_SOURCE_DIR}/src/vm/prim.h"
            "${PROJECT_SOURCE_DIR}/src/vm/version.h"
)

SET_SOURCE_FILES_PROPERTIES( "${COMMON_SRCS}" PROPERTIES LANGUAGE C )

# old bootstrap
add_executable(bootstrap   "${PROJECT_SOURCE_DIR}/src/bootstrap/bootstrap.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/err.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/err.h"
                           "${PROJECT_SOURCE_DIR}/src/vm/globals.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/globals.h"
                           "${PROJECT_SOURCE_DIR}/src/vm/image.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/image.h"
                           "${PROJECT_SOURCE_DIR}/src/vm/interp.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
                           "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                           "${PROJECT_SOURCE_DIR}/src/vm/prim.c"
                           "${PROJECT_SOURCE_DIR}/src/vm/prim.h"
                           "${PROJECT_SOURCE_DIR}/src/vm/version.h"
              )


add_executable(lst "${PROJECT_SOURCE_DIR}/src/vm/main.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/err.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/err.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/globals.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/globals.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/image.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/image.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/prim.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/prim.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/version.h"
              )


# bootstrap the initial image
add_custom_target(baseimage ALL
    COMMAND bootstrap -v ${PROJECT_SOURCE_DIR}/src/smalltalk/system/lst.st -o "${CMAKE_BINARY_DIR}/lst_repl.img"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Bootstrapping initial image file." VERBATIM
    SOURCES ${PROJECT_SOURCE_DIR}/src/smalltalk/system/lst.st
)

# bootstrap the web IDE image
add_custom_target(ideimage ALL
    COMMAND bootstrap -v ${PROJECT_SOURCE_DIR}/src/smalltalk/webide/webide.st -o "${CMAKE_BINARY_DIR}/lst_webide.img"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Bootstrapping web IDE image file." VERBATIM
    SOURCES ${PROJECT_SOURCE_DIR}/src/smalltalk/webide/webide.st
)

