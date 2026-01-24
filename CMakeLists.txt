cmake_minimum_required(VERSION 3.20)

project(file_indexer
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(file_indexer
    src/main.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(file_indexer PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

target_include_directories(file_indexer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)