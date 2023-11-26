cmake_minimum_required(VERSION 3.14)
project(svg-reader VERSION 0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(Git)
message(STATUS "GIT_EXECUTABLE='${GIT_EXECUTABLE}'")

find_package(sfml QUIET)

add_subdirectory(external/SFML)
include_directories(src)
if (LINUX)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()
file(GLOB_RECURSE cpp_files CONFIGURE_DEPENDS "src/*.*pp")
add_executable(${PROJECT_NAME} ${cpp_files})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
