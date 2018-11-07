cmake_minimum_required(VERSION 3.11)
project(misaxx)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)

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
        GIT_TAG origin/rework
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
        misaxx/filesystem/misa_filesystem_directories_importer.h
        misaxx/misa_module.h
        misaxx/misa_module_declaration_base.h
        misaxx/workers/misa_task.h
        misaxx/workers/misa_worker.h
        misaxx/misa_module_base.h
        misaxx/workers/misa_dispatcher.h
        misaxx/misa_cached_data.h
        misaxx/misa_submodule.h
        misaxx/runtime/misa_runtime.h
        misaxx/filesystem/misa_filesystem.h
        misaxx/misa_module_declaration.h
        misaxx/runtime/misa_cli.h
        misaxx/caches/misa_unsafe_image_stack.h
        misaxx/caches/misa_unsafe_file.h
        misaxx/caches/misa_unsafe_image_file.h
        misaxx/filesystem/misa_filesystem_json_importer.h
        misaxx/misa_multiobject_root.h
        misaxx/parameters/algorithm_node_path.h
        misaxx/misa_root_module_base.h
        misaxx/parameters/object_node_path.h
        misaxx/attachments/object2d_pixels.h
        misaxx/attachments/object3d_pixels.h
        misaxx/caches/misa_unsafe_json_file.h
        misaxx/attachments/object3d_volume.h
        misaxx/caches/misa_unsafe_exportable_meta_data.h
        misaxx/misa_serializeable.h
        misaxx/attachments/object3d_voxel_bounds.h
        misaxx/attachments/object3d_voxel_size.h
        misaxx/misa_future_dispatch.h
        misaxx/attachments/object_name.h
        misaxx/filesystem/misa_filesystem_empty_importer.h
        misaxx/misa_description_storage.h
        misaxx/misa_cache.h
        misaxx/descriptions/misa_file_description.h
        misaxx/descriptions/misa_file_stack_description.h
        misaxx/caches/misa_image_file.h
        misaxx/patterns/misa_file_pattern.h
        misaxx/misa_data_pattern.h
        misaxx/misa_data_description.h
        misaxx/patterns/misa_file_stack_pattern.h
        misaxx/patterns/misa_image_file_stack_pattern.h
        misaxx/descriptions/misa_image_file_description.h
        misaxx/patterns/misa_image_file_pattern.h
        misaxx/misa_module_declaration.cpp
        misaxx/misa_cached_data.cpp
        misaxx/misa_submodule.cpp
        misaxx/misa_cache.cpp
        misaxx/misa_root_module_base.cpp
        misaxx/parameters/algorithm_node_path.cpp
        misaxx/misa_module.cpp
        misaxx/misa_data_description.cpp
        misaxx/runtime/misa_runtime.cpp
        misaxx/parameters/object_node_path.cpp
        misaxx/misa_multiobject_root.cpp
        misaxx/misa_module_declaration_base.cpp
        misaxx/workers/misa_task.cpp
        misaxx/misa_future_dispatch.cpp
        misaxx/misa_module_base.cpp
        misaxx/misa_serializeable.cpp
        misaxx/misa_description_storage.cpp
        misaxx/workers/misa_dispatcher.cpp
        misaxx/misa_data_pattern.cpp
        misaxx/runtime/misa_cli.cpp
        misaxx/workers/misa_worker.cpp
        misaxx/attachments/object3d_pixels.cpp
        misaxx/attachments/object2d_pixels.cpp
        misaxx/attachments/object3d_voxel_size.cpp
        misaxx/attachments/object_name.cpp
        misaxx/attachments/object3d_volume.cpp
        misaxx/attachments/object3d_voxel_bounds.cpp
        misaxx/patterns/misa_image_file_stack_pattern.cpp
        misaxx/patterns/misa_file_pattern.cpp
        misaxx/patterns/misa_image_file_pattern.cpp
        misaxx/patterns/misa_file_stack_pattern.cpp
        misaxx/descriptions/misa_file_stack_description.cpp
        misaxx/descriptions/misa_image_file_description.cpp
        misaxx/descriptions/misa_file_description.cpp
        misaxx/caches/misa_unsafe_image_stack.cpp
        misaxx/caches/misa_image_file.cpp
        misaxx/caches/misa_unsafe_json_file.cpp
        misaxx/caches/misa_unsafe_image_file.cpp
        misaxx/caches/misa_unsafe_exportable_meta_data.cpp
        misaxx/caches/misa_unsafe_file.cpp
        misaxx/filesystem/misa_filesystem_json_importer.cpp
        misaxx/filesystem/misa_filesystem_entry.cpp
        misaxx/filesystem/misa_filesystem_directories_importer.cpp
        misaxx/filesystem/misa_filesystem_empty_importer.cpp
        misaxx/filesystem/misa_filesystem.cpp misaxx/detail/misa_json_schema.h misaxx/detail/misa_json_schema.cpp)

target_include_directories(misaxx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(misaxx PUBLIC
        Boost::filesystem
        Boost::regex
        Boost::program_options
        cxx-helpers
        pattxx
        coixx)

add_executable(misaxx_test main.cpp)
target_link_libraries(misaxx_test misaxx)
