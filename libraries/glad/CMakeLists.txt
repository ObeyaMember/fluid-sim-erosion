add_library(glad glad.c)
target_include_directories(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()