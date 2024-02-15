cmake_minimum_required(VERSION 3.12)
project(Pathtracer)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenMP REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "Sources/*.cpp" "External/ImGui/*.cpp" "External/tinyfiledialogs.c")

include_directories(
    External
    External/ImGui
    External/ImGui/Backends
    External/Libs/glfw/include
    External/stb_image
    External/tinyfiledialogs
)

add_executable(Pathtracer ${SOURCE_FILES})

target_link_libraries(Pathtracer
    glfw
    OpenGL::GL
    OpenMP::OpenMP_CXX
    ${CMAKE_DL_LIBS}
)