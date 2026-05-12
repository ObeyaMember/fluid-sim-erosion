add_library(mesh_renderer mesh_renderer.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(mesh_renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()