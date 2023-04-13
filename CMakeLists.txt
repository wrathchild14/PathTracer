cmake_minimum_required(VERSION 3.0.0)
project(PathTracer VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(ThirdParty/Imgui)

add_executable(PathTracer main.cpp)

target_link_libraries(PathTracer PRIVATE imgui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
