add_library(transforms transforms.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(transforms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()