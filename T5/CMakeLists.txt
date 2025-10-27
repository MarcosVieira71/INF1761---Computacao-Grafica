cmake_minimum_required(VERSION 3.10)
project(T5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/../commons ${CMAKE_BINARY_DIR}/commons_build)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glew/include
    ${CMAKE_SOURCE_DIR}/external/glm
)

file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(T5 ${SOURCES})

target_link_libraries(T5 PRIVATE commons)

if(WIN32)
    target_link_libraries(T5 PRIVATE glfw glew32 opengl32)
elseif(APPLE)
    find_library(COCOA Cocoa REQUIRED)
    find_library(IOKIT IOKit REQUIRED)
    find_library(COREVIDEO CoreVideo REQUIRED)
    find_library(OPENGL OpenGL REQUIRED)
    target_link_libraries(T5 PRIVATE glfw GLEW ${OPENGL} ${COCOA} ${IOKIT} ${COREVIDEO})
else() # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(T5 PRIVATE glfw GLEW ${OPENGL_LIBRARIES} dl pthread)
endif()
