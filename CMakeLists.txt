cmake_minimum_required(VERSION 3.10.0)
project(TIPE_2.0 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


# Libraries
add_subdirectory(./libraries/default_libs)
add_subdirectory(./libraries/cglm/ EXCLUDE_FROM_ALL)
add_subdirectory(./libraries/glad/)
#add_subdirectory(./libraries/GLFW)
add_subdirectory(./libraries/stb_image)
add_subdirectory(./libraries/utilities/shaders/)
add_subdirectory(./libraries/utilities/gl_context)
add_subdirectory(./libraries/utilities/data)
add_subdirectory(./libraries/utilities/camera)
add_subdirectory(./libraries/utilities/primitives)
add_subdirectory(./libraries/utilities/transforms)
add_subdirectory(./libraries/utilities/)

# Scenes
add_subdirectory(./scenes/default_scene/)

set(SourceFiles "./main.c")
list(APPEND SourceFiles "./libraries/glad/glad.c")
list(APPEND SourceFiles "./libraries/utilities/shaders/shaders.c")
list(APPEND SourceFiles "./libraries/utilities/data/data.c")
list(APPEND SourceFiles "./libraries/utilities/gl_context/gl_context.c")
list(APPEND SourceFiles "./libraries/utilities/camera/camera.c")
list(APPEND SourceFiles "./libraries/utilities/primitives/primitives.c")
list(APPEND SourceFiles "./libraries/utilities/transforms/transforms.c")
list(APPEND SourceFiles "./libraries/utilities/data/data.c")

add_executable(${CMAKE_PROJECT_NAME} ${SourceFiles})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "TIPE.exe")
#hmmmmmmmmmmmmmmmmmmmmmmmmmm - why does glad, in particular, require this#include "camera/camera.h"

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC m)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC cglm)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC default_libs)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC stb_image)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC glad)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC shaders)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC data)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC gl_context)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC camera)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC primitives)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC transforms)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC utilities)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC default_scene)

