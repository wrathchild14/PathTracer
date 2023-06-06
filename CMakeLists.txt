cmake_minimum_required(VERSION 3.12)
project(Pathtracer)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES "Source/*.cpp")

include_directories(
    External
    External/ImGui
    External/glfw/include
    External/stb_image
    External/tinyfiledialogs
)

add_executable(Pathtracer ${SOURCE_FILES})

target_link_libraries(Pathtracer
    ${CMAKE_SOURCE_DIR}/External/glfw/libglfw.a
)
