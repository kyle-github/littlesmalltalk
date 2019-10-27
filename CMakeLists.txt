cmake_minimum_required(VERSION 2.8)

set( PROJECT_NAME "LittleSmalltalk" )

project(${PROJECT_NAME})

add_executable(bootstrap "${PROJECT_SOURCE_DIR}/src/bootstrap/bootstrap.c"
                         "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                         "${PROJECT_SOURCE_DIR}/src/vm/interp.h" )

add_executable(bootstrap3 "${PROJECT_SOURCE_DIR}/src/vm_new/array.c"
                          "${PROJECT_SOURCE_DIR}/src/vm_new/array.h"
                          "${PROJECT_SOURCE_DIR}/src/vm_new/bootstrap3.c"
                          "${PROJECT_SOURCE_DIR}/src/vm_new/memory.c"
                          "${PROJECT_SOURCE_DIR}/src/vm_new/memory.h"
                          "${PROJECT_SOURCE_DIR}/src/vm_new/globals.c"
                          "${PROJECT_SOURCE_DIR}/src/vm_new/globals.h"
)


# add_executable(bootstrap2 "${PROJECT_SOURCE_DIR}/src/bootstrap/bootstrap2.c")

add_executable(lst "${PROJECT_SOURCE_DIR}/src/vm/main.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/err.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/err.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/globs.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/prim.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/prim.h" )

#add_custom_command(
#    OUTPUT  lst.img
#    COMMAND bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource lst.img
#    DEPENDS bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource
#)

add_custom_target(image ALL
    COMMAND bootstrap ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource lst.img
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Bootstrapping initial image file." VERBATIM
    SOURCES ${PROJECT_SOURCE_DIR}/src/bootstrap/imageSource
)
