add_library(general_purpose general_purpose.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(general_purpose PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()