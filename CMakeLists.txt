cmake_minimum_required(VERSION 3.11)
project(misaxx)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP)

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.1.2/include.zip
)
FetchContent_Declare(
        coixx
        GIT_REPOSITORY https://asb-git.hki-jena.de/RGerst/coixx.git
        GIT_TAG origin/rework2
)
FetchContent_Declare(
        cxxh
        GIT_REPOSITORY https://asb-git.hki-jena.de/RGerst/cxx-helpers.git
        GIT_TAG origin/rework
)

FetchContent_GetProperties(nlohmann_json)
if(NOT nlohmann_json_POPULATED)
    message("-- Fetching dependency nlohmann_json ...")
    FetchContent_Populate(nlohmann_json)
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR})
endif()
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

add_library(misaxx
        misaxx/attachments/object3d_pixels.h
        misaxx/attachments/object2d_pixels.h
        misaxx/attachments/object3d_voxel_size.h
        misaxx/attachments/object_name.h
        misaxx/attachments/object3d_volume.h
        misaxx/attachments/object3d_voxel_bounds.h
        misaxx/patterns/misa_image_file_stack_pattern.h
        misaxx/patterns/misa_file_pattern.h
        misaxx/patterns/misa_image_file_pattern.h
        misaxx/patterns/misa_file_stack_pattern.h
        misaxx/misa_module_declaration.h
        misaxx/misa_cached_data.h
        misaxx/misa_submodule.h
        misaxx/misa_cache.h
        misaxx/misa_root_module_base.h
        misaxx/parameters/parameter_prepare.h
        misaxx/parameters/parameter_base.h
        misaxx/parameters/default_parameter_check.h
        misaxx/parameters/default_parameter_prepare.h
        misaxx/parameters/input_parameter.h
        misaxx/parameters/output_parameter.h
        misaxx/parameters/parameter_checks.h
        misaxx/misa_module.h
        misaxx/misa_data_pattern_base.h
        misaxx/misa_data_description.h
        misaxx/misa_multiobject_root.h
        misaxx/descriptions/misa_file_stack_description.h
        misaxx/descriptions/misa_image_file_description.h
        misaxx/descriptions/misa_file_description.h
        misaxx/misa_module_declaration_base.h
        misaxx/json/misa_json_schema_builder.h
        misaxx/json/misa_json_schema.h
        misaxx/json/misa_json_property.h
        misaxx/json/misa_json_property_base.h
        misaxx/caches/misa_unsafe_image_stack.h
        misaxx/caches/misa_image_file.h
        misaxx/caches/misa_unsafe_json_file.h
        misaxx/caches/misa_unsafe_image_file.h
        misaxx/caches/misa_exported_attachments.h
        misaxx/caches/misa_unsafe_file.h
        misaxx/filesystem/misa_filesystem_json_importer.h
        misaxx/filesystem/misa_filesystem_entry.h
        misaxx/filesystem/misa_filesystem_directories_importer.h
        misaxx/filesystem/misa_filesystem_empty_importer.h
        misaxx/filesystem/misa_filesystem.h
        misaxx/misa_future_dispatch.h
        misaxx/misa_module_base.h
        misaxx/runtime/misa_runtime_base.h
        misaxx/runtime/misa_runtime.h
        misaxx/runtime/misa_cli.h
        misaxx/misa_serializeable.h
        misaxx/misa_description_storage.h
        misaxx/workers/paths/algorithm_node_path.h
        misaxx/workers/paths/object_node_path.h
        misaxx/workers/paths/guarded_node_path.h
        misaxx/workers/paths/full_node_path.h
        misaxx/workers/misa_worker_base.h
        misaxx/workers/task_tree/misa_work_node.h
        misaxx/workers/task_tree/misa_work_subtree_status.h
        misaxx/workers/task_tree/misa_worker_status.h
        misaxx/workers/misa_task.h
        misaxx/workers/misa_functional_task.h
        misaxx/workers/dependency_management/misa_work_dependency_group.h
        misaxx/workers/dependency_management/misa_work_dependency_chain.h
        misaxx/workers/dependency_management/misa_work_dependency_segment.h
        misaxx/workers/misa_dispatcher.h
        misaxx/workers/misa_worker.h
        misaxx/misa_data_pattern.h
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
        misaxx/misa_module_declaration.cpp
        misaxx/misa_cached_data.cpp
        misaxx/misa_submodule.cpp
        misaxx/misa_cache.cpp
        misaxx/misa_root_module_base.cpp
        misaxx/parameters/parameter_prepare.cpp
        misaxx/parameters/parameter_base.cpp
        misaxx/parameters/default_parameter_check.cpp
        misaxx/parameters/default_parameter_prepare.cpp
        misaxx/parameters/input_parameter.cpp
        misaxx/parameters/output_parameter.cpp
        misaxx/parameters/parameter_checks.cpp
        misaxx/misa_module.cpp
        misaxx/misa_data_pattern_base.cpp
        misaxx/misa_data_description.cpp
        misaxx/misa_multiobject_root.cpp
        misaxx/descriptions/misa_file_stack_description.cpp
        misaxx/descriptions/misa_image_file_description.cpp
        misaxx/descriptions/misa_file_description.cpp
        misaxx/misa_module_declaration_base.cpp
        misaxx/json/misa_json_schema_builder.cpp
        misaxx/json/misa_json_schema.cpp
        misaxx/json/misa_json_property.cpp
        misaxx/json/misa_json_property_base.cpp
        misaxx/caches/misa_unsafe_image_stack.cpp
        misaxx/caches/misa_image_file.cpp
        misaxx/caches/misa_unsafe_json_file.cpp
        misaxx/caches/misa_unsafe_image_file.cpp
        misaxx/caches/misa_exported_attachments.cpp
        misaxx/caches/misa_unsafe_file.cpp
        misaxx/filesystem/misa_filesystem_json_importer.cpp
        misaxx/filesystem/misa_filesystem_entry.cpp
        misaxx/filesystem/misa_filesystem_directories_importer.cpp
        misaxx/filesystem/misa_filesystem_empty_importer.cpp
        misaxx/filesystem/misa_filesystem.cpp
        misaxx/misa_future_dispatch.cpp
        misaxx/misa_module_base.cpp
        misaxx/runtime/misa_runtime_base.cpp
        misaxx/runtime/misa_runtime.cpp
        misaxx/runtime/misa_cli.cpp
        misaxx/misa_serializeable.cpp
        misaxx/misa_description_storage.cpp
        misaxx/workers/paths/algorithm_node_path.cpp
        misaxx/workers/paths/object_node_path.cpp
        misaxx/workers/paths/guarded_node_path.cpp
        misaxx/workers/paths/full_node_path.cpp
        misaxx/workers/misa_worker_base.cpp
        misaxx/workers/task_tree/misa_work_node.cpp
        misaxx/workers/task_tree/misa_work_subtree_status.cpp
        misaxx/workers/task_tree/misa_worker_status.cpp
        misaxx/workers/misa_task.cpp
        misaxx/workers/dependency_management/misa_work_dependency_group.cpp
        misaxx/workers/dependency_management/misa_work_dependency_chain.cpp
        misaxx/workers/dependency_management/misa_work_dependency_segment.cpp
        misaxx/workers/misa_dispatcher.cpp
        misaxx/workers/misa_worker.cpp
        misaxx/misa_data_pattern.cpp misaxx/misa_primitive.h misaxx/misa_serialization_id.h misaxx/json/misa_json_helper.h misaxx/misa_cached_data_base.cpp misaxx/misa_cached_data_base.h misaxx/misa_default_cache.h misaxx/attachments/filesystem_link.cpp misaxx/attachments/filesystem_link.h)

target_include_directories(misaxx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(misaxx PUBLIC
        Boost::filesystem
        Boost::regex
        Boost::program_options
        OpenMP::OpenMP_CXX
        nlohmann_json
        cxx-helpers
        coixx)

add_executable(misaxx_test main.cpp)
target_link_libraries(misaxx_test misaxx)
