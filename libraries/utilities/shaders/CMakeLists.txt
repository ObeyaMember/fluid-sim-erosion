add_library(shaders shaders.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(shaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()