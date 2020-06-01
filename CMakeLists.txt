cmake_minimum_required (VERSION 3.10)
project(Shadertoy)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(ZyEngine)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

add_executable(Shadertoy ${SOURCES} ${HEADERS})
target_link_libraries(Shadertoy LINK_PUBLIC ZyEngine)