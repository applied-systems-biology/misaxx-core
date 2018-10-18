cmake_minimum_required(VERSION 3.11)
project(misaxx)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP)

FetchContent_Declare(
        coixx
        GIT_REPOSITORY https://asb-git.hki-jena.de/RGerst/coixx.git
        GIT_TAG origin/master
)
FetchContent_Declare(
        cxxh
        GIT_REPOSITORY https://asb-git.hki-jena.de/RGerst/cxx-helpers.git
        GIT_TAG origin/master
)
FetchContent_Declare(
        pattxx
        GIT_REPOSITORY https://asb-git.hki-jena.de/RGerst/pattxx.git
        GIT_TAG origin/master
)

FetchContent_GetProperties(coixx)
if(NOT coixx_POPULATED)
    message("-- Fetching dependency coixx ...")
    FetchContent_Populate(coixx)
    add_subdirectory(${coixx_SOURCE_DIR} ${coixx_BINARY_DIR})
endif()
FetchContent_GetProperties(cxxh)
if(NOT cxxh_POPULATED)
    message("-- Fetching dependency cxxh ...")
    FetchContent_Populate(cxxh)
    add_subdirectory(${cxxh_SOURCE_DIR} ${cxxh_BINARY_DIR})
endif()
FetchContent_GetProperties(pattxx)
if(NOT pattxx_POPULATED)
    message("-- Fetching dependency pattxx ...")
    FetchContent_Populate(pattxx)
    add_subdirectory(${pattxx_SOURCE_DIR} ${pattxx_BINARY_DIR})
endif()

add_library(misaxx
        misaxx/filesystem/misa_filesystem_entry.h
        misaxx/filesystem/directories_importer.h
        misaxx/misa_module.h
        misaxx/pdata/misa_file_stack.h
        misaxx/misa_module_declaration_base.h
        misaxx/misa_task.h
        misaxx/misa_worker.h
        misaxx/misa_module_base.h
        misaxx/misa_dispatcher.h
        misaxx/pdata/misa_pdata.h
        misaxx/misa_submodule.h
        misaxx/misa_runtime.h
        misaxx/filesystem/misa_filesystem.h
        misaxx/misa_module_declaration.h
        misaxx/misa_cli.h
        misaxx/pdata/misa_image_stack.h
        misaxx/pdata/misa_file.h
        misaxx/pdata/misa_generic_file_stack.h
        misaxx/pdata/misa_image_file.h
        misaxx/filesystem/json_importer.h
        misaxx/misa_multiobject_root.h
        misaxx/algorithm_node_path.h
        misaxx/misa_root_module_base.h
        misaxx/object_node_path.h
        misaxx/vdata/object2d_pixels.h
        misaxx/vdata/object3d_pixels.h
        misaxx/pdata/misa_json_file.h
        misaxx/vdata/object3d_volume.h
        misaxx/pdata/misa_exportable_meta_data.h
        misaxx/vdata/misa_metadata.h
        misaxx/vdata/object3d_voxel_bounds.h
        misaxx/vdata/object3d_voxel_size.h
        misaxx/misa_future_dispatch.h
        misaxx/vdata/object_name.h
        misaxx/filesystem/empty_importer.h
        misaxx/filesystem/json_schema_serializer.h)

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
