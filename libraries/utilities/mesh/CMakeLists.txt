add_library(mesh mesh.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()