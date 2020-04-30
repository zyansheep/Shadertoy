cmake_minimum_required (VERSION 3.10)
project(Shadertoy)

set(CMAKE_CXX_STANDARD 17)

include_directories(ZyEngine/src)
include_directories(ZyEngine/libs/glad/include)
include_directories(ZyEngine/libs/imgui)
include_directories(ZyEngine/libs/glm)
add_subdirectory(ZyEngine)

#Find Sources and Headers in src folder
file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.h)

#Test executable
add_executable(Shadertoy ${SOURCES} ${HEADERS})
target_link_libraries(Shadertoy LINK_PUBLIC ZyEngine)