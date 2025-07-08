cmake_minimum_required(VERSION 3.10.0)
project(TIPE_2.0 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})



add_subdirectory(./libraries/default_libs)
add_subdirectory(./libraries/cglm/ EXCLUDE_FROM_ALL)
add_subdirectory(./libraries/glad/)
#add_subdirectory(./libraries/GLFW)
add_subdirectory(./libraries/utilities/shaders)
add_subdirectory(./libraries/utilities/data)
add_subdirectory(./libraries/utilities/gl_context)
add_subdirectory(./libraries/utilities/)

add_executable(TIPE_2.0 ./main.c ./libraries/glad/glad.c) 
set_target_properties(TIPE_2.0 PROPERTIES OUTPUT_NAME "TIPE.exe")
#hmmmmmmmmmmmmmmmmmmmmmmmmmm - why does glad, in particular, require this

target_link_libraries(TIPE_2.0 PUBLIC m)
target_link_libraries(TIPE_2.0 PUBLIC cglm)
target_link_libraries(TIPE_2.0 PUBLIC default_libs)
target_link_libraries(TIPE_2.0 PUBLIC glad)
target_link_libraries(TIPE_2.0 PUBLIC glfw)
target_link_libraries(TIPE_2.0 PUBLIC shaders)
target_link_libraries(TIPE_2.0 PUBLIC data)
target_link_libraries(TIPE_2.0 PUBLIC gl_context)
target_link_libraries(TIPE_2.0 PUBLIC utilities)

