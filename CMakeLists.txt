cmake_minimum_required(VERSION 2.8)

set( PROJECT_NAME "lst" )

project(${PROJECT_NAME})

# the project is version 4.7
set (lst_VERSION_MAJOR 4)
set (lst_VERSION_MINOR 7)
set (lst_VERSION_PATCH 0)
set (VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

# generate version file from CMake info.
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/src/vm/version.h.in" "${PROJECT_SOURCE_DIR}/src/vm/version.h" @ONLY)


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
                   "${PROJECT_SOURCE_DIR}/src/vm/version.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/version.h"
              )


# gather the Smalltalk sources.
#file(GLOB_RECURSE st_sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/smalltalk/system/*.st")

#message("Smalltalk source files: ${st_sources}.")

# set up the boot strapping target to make the image.

# old bootstrap
#add_custom_target(image ALL
#    COMMAND bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource lst.img
#    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
#    COMMENT "Bootstrapping initial image file." VERBATIM
#    SOURCES ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource
#)


# new bootstrap
#add_custom_target(image ALL
#    COMMAND bootstrap ${st_sources}
#    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
#    COMMENT "Bootstrapping initial image file, lst.img." VERBATIM
#    SOURCES ${st_sources}
#)

# old bootstrap
#add_custom_command(
#    OUTPUT  lst.img
#    COMMAND bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource lst.img
#    DEPENDS bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource
#)

