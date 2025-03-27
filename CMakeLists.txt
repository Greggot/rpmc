
cmake_minimum_required(VERSION 3.5)
project(lcad)

file(GLOB_RECURSE SOURCES "src/*.c")
SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES LANGUAGE C)

add_executable(rpmc main.c ${SOURCES})

target_include_directories(rpmc PUBLIC "src")

target_link_libraries(rpmc m)

target_compile_options(rpmc PRIVATE 
    -Wall 
    -Wextra 
    -pedantic 
    -Wno-unused-macros 
    -Wno-unused-function 
    -Wno-unreachable-code 
    -Wno-padded 
    -Wno-reserved-identifier 
    -Wuninitialized
)

set_property(TARGET rpmc PROPERTY C_STANDARD 17)
