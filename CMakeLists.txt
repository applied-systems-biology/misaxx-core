include(FetchContent)
cmake_minimum_required(VERSION 3.11)
project(misaxx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP)

add_library(misaxx
        misaxx/test.cpp
        misaxx/filesystem/importers/json_importer.h
        misaxx/filesystem/misa_stack.h
        misaxx/filesystem/misa_filesystem.h
        misaxx/filesystem/misa_folder.h misaxx/filesystem/misa_entry.h misaxx/filesystem/importers/default_importer.h)

if(NOT TARGET cxx-helpers)
    add_subdirectory(libs/cxx-helpers)
endif()

target_include_directories(misaxx SYSTEM BEFORE PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(misaxx BEFORE PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(misaxx INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(misaxx PUBLIC
        ${Boost_LIBRARIES}
        ${OpenMP_CXX_LIBRARIES}
        cxx-helpers)

target_compile_options(misaxx PUBLIC ${OpenMP_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -pedantic-errors)

add_executable(misaxx_test main.cpp)
target_link_libraries(misaxx_test misaxx)
