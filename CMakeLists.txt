cmake_minimum_required(VERSION 3.14)
project(svg-reader VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(sfml QUIET)
if (NOT sfml_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		SFML
		GIT_REPOSITORY https://github.com/SFML/SFML.git
		GIT_TAG 2.6.x
	)
	FetchContent_MakeAvailable(SFML)
endif()

add_subdirectory(external/pugixml)
include_directories(src)
file(GLOB_RECURSE cpp_files CONFIGURE_DEPENDS "src/*.*pp")
add_executable(${PROJECT_NAME} ${cpp_files})

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics)
target_link_libraries(${PROJECT_NAME} PUBLIC pugixml)
