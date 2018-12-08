cmake_minimum_required(VERSION 2.8)

set( PROJECT_NAME "LittleSmalltalk")

project(${PROJECT_NAME})

add_executable(bootstrap "${PROJECT_SOURCE_DIR}/src/bootstrap/bootstrap.c"
                         "${PROJECT_SOURCE_DIR}/src/vm/memory.h"
                         "${PROJECT_SOURCE_DIR}/src/vm/interp.h" )

add_executable(lst "${PROJECT_SOURCE_DIR}/src/vm/main.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/globs.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/interp.h"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.c"
                   "${PROJECT_SOURCE_DIR}/src/vm/memory.h" )

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
