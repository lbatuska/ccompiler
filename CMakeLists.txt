cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ccompiler LANGUAGES C)

set(CMAKE_C_STANDARD 11)

file(GLOB COMPILER_SRC CONFIGURE_DEPENDS "./*.c")
# include_directories("./")

add_executable(ccompiler main.c ${COMPILER_SRC})
add_subdirectory(cstd)
target_link_libraries(ccompiler PRIVATE cstd)

