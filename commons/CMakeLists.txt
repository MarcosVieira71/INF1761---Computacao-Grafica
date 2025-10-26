cmake_minimum_required(VERSION 3.10)

project(commons)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB COMMONS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(commons STATIC ${COMMONS_SOURCES})

target_include_directories(commons PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
