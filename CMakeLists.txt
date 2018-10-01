cmake_minimum_required(VERSION 3.11)
project(misaxx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP)

add_library(misaxx
        misaxx/filesystem/vfs_folder.h
        misaxx/filesystem/vfs_entry.h
        misaxx/filesystem/importers/directories_importer.h
        misaxx/filesystem/vfs_file.h
        misaxx/misa_module.h
        misaxx/module_data/misa_file_stack.h
        misaxx/misa_module_declaration_base.h
        misaxx/misa_task.h
        misaxx/misa_worker.h
        misaxx/misa_module_base.h
        misaxx/misa_dispatcher.h
        misaxx/misa_module_data.h
        misaxx/misa_submodule.h
        misaxx/misa_runtime.h
        misaxx/misa_filesystem.h
        misaxx/misa_module_declaration.h
        misaxx/misa_cli.h
        misaxx/module_data/misa_image_stack.h
        misaxx/module_data/misa_file.h
        misaxx/module_data/misa_generic_file_stack.h
        misaxx/module_data/misa_image_file.h
        misaxx/filesystem/importers/json_importer.h
        misaxx/root_modules/misa_multiobject_root.h
        misaxx/algorithm_node_path.h
        misaxx/root_modules/misa_root_module_base.h
        misaxx/object_node_path.h
        misaxx/metadata/object2d_pixels.h
        misaxx/metadata/object3d_pixels.h
        misaxx/module_data/misa_json_file.h
        misaxx/metadata/object3d_volume.h
        misaxx/module_data/misa_exportable_meta_data.h
        misaxx/metadata/misa_metadata.h
        misaxx/metadata/object3d_voxel_bounds.h
        misaxx/metadata/object3d_voxel_size.h
        misaxx/misa_future_dispatch.h
        misaxx/metadata/object_name.h)

include(cmake_modules/find_cxxh.cmake)
include(cmake_modules/find_pattxx.cmake)
include(cmake_modules/find_coixx.cmake)

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
