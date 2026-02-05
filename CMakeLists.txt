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

target_sources(file_indexer PRIVATE
    src/index.cpp
    src/cli.cpp
    src/query.cpp
    src/scan.cpp
    src/file_record.cpp
    src/index_io.cpp
    src/index_file.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(file_indexer PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-unsafe-buffer-usage
        -Wno-padded
    )
endif()

target_include_directories(file_indexer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)