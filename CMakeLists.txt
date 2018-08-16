include(FetchContent)
cmake_minimum_required(VERSION 3.11)
project(misaxx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP)

add_library(misaxx
        misaxx/filesystem/importers/json_importer.h
        misaxx/filesystem/virtual_filesystem.h
        misaxx/filesystem/vfs_folder.h misaxx/filesystem/vfs_entry.h misaxx/filesystem/importers/folder_importer.h misaxx/filesystem/vfs_file.h misaxx/misa_module.h misaxx/misa_file_stack.h misaxx/misa_module_definition.h misaxx/misa_task.h misaxx/misa_worker.h misaxx/misa_module_base.h misaxx/misa_dispatcher.h misaxx/misa_module_data.h misaxx/misa_submodule.h misaxx/misa_runtime.h)

if(NOT TARGET cxx-helpers)
    add_subdirectory(libs/cxx-helpers)
endif()
if(NOT TARGET pattxx)
    add_subdirectory(libs/pattxx)
endif()
if(NOT TARGET coixx)
    add_subdirectory(libs/coixx)
endif()


target_include_directories(misaxx SYSTEM BEFORE PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(misaxx BEFORE PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(misaxx INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(misaxx PUBLIC
        ${Boost_LIBRARIES}
        ${OpenMP_CXX_LIBRARIES}
        cxx-helpers
        pattxx
        coixx)

target_compile_options(misaxx PUBLIC ${OpenMP_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -pedantic-errors)

add_executable(misaxx_test main.cpp)
target_link_libraries(misaxx_test misaxx)
