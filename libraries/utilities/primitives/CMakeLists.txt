add_library(primitives primitives.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(primitives PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()