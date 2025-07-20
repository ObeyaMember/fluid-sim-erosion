add_library(fluid_simulation fluid_simulation.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(fluid_simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()