add_library(gl_context gl_context.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(gl_context PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()