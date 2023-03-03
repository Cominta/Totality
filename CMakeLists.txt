cmake_minimum_required(VERSION 3.2)
project(Totality)

set(CMAKE_CXX_STANDARD 17)
set(Totality_VERSION 0.1)
set(EXECUTABLE_OUTPUT_PATH ../output/)
# set( CMAKE_DISABLE_FIND_PACKAGE_PkgConfig ON)

configure_file("${PROJECT_SOURCE_DIR}/include/versionToGen.h" "${PROJECT_SOURCE_DIR}/genCmake/version.h")
configure_file("${PROJECT_SOURCE_DIR}/include/pathToGen.h" "${PROJECT_SOURCE_DIR}/genCmake/path.h")

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/genCmake")

file (GLOB CPP_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libsfml-system-d.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libsfml-graphics-d.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libsfml-window-d.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libsfml-main-d.a)