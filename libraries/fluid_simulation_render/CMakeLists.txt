add_library(fluid_simulation_render fluid_simulation_render.c) # I'VE NO CLUE IF THIS WORKS AND I SHALL NOT CHECK FOR NOW

target_include_directories(fluid_simulation_render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()