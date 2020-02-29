cmake_minimum_required(VERSION 2.8)

set( PROJECT_NAME "LittleSmalltalk" )

project(${PROJECT_NAME})

# old bootstrap
add_executable(bootstrap "${PROJECT_SOURCE_DIR}/src/bootstrap/bootstrap.c"
                         "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                         "${PROJECT_SOURCE_DIR}/src/vm/globals.h"
                         "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
              )


# new bootstrap
#add_executable(bootstrap "${PROJECT_SOURCE_DIR}/src/vm/bootstrap.c"
#                   "${PROJECT_SOURCE_DIR}/src/vm/err.c"
#                   "${PROJECT_SOURCE_DIR}/src/vm/err.h"
#                   "${PROJECT_SOURCE_DIR}/src/vm/globals.c"
#                   "${PROJECT_SOURCE_DIR}/src/vm/globals.h"
#                   "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
#                   "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
#              )


add_executable(lst "${PROJECT_SOURCE_DIR}/src/vm/main.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/err.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/err.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/globals.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/globals.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/prim.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/prim.h"
              )


# gather the Smalltalk sources.
#file(GLOB_RECURSE st_sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/smalltalk/system/*.st")

#message("Smalltalk source files: ${st_sources}.")

# set up the boot strapping target to make the image.

# old bootstrap
add_custom_target(image ALL
    COMMAND bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource lst.img
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Bootstrapping initial image file." VERBATIM
    SOURCES ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource
)


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

