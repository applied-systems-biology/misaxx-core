cmake_minimum_required(VERSION 3.11)
project(misaxx_core LANGUAGES CXX VERSION 0.1.0)

include(FetchContent)
include(FeatureSummary)
include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Build options
option(BUILD_SHARED_LIBS "Build shared library" ON)

# Dependencies
find_package(misaxx-helpers REQUIRED)
find_package(misaxx-coixx REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
find_package(OpenMP REQUIRED)
feature_summary(WHAT ALL)

# Setting up the library
add_library(misaxx-core
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
        misaxx/descriptions/misa_file_description.h
        misaxx/misa_module_declaration_base.h
        misaxx/json/misa_json_schema_builder.h
        misaxx/json/misa_json_schema.h
        misaxx/json/misa_json_property.h
        misaxx/json/misa_json_property_base.h
        misaxx/caches/misa_image_stack_cache.h
        misaxx/caches/misa_image_file_cache.h
        misaxx/caches/misa_exported_attachments_cache.h
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
        misaxx/descriptions/misa_file_description.cpp
        misaxx/misa_module_declaration_base.cpp
        misaxx/json/misa_json_schema_builder.cpp
        misaxx/json/misa_json_schema.cpp
        misaxx/json/misa_json_property.cpp
        misaxx/json/misa_json_property_base.cpp
        misaxx/caches/misa_image_stack_cache.cpp
        misaxx/caches/misa_image_file_cache.cpp
        misaxx/caches/misa_exported_attachments_cache.cpp
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
        misaxx/misa_data_pattern.cpp
        misaxx/misa_primitive.h
        misaxx/misa_serialization_id.h
        misaxx/json/misa_json_helper.h
        misaxx/misa_cached_data_base.cpp
        misaxx/misa_cached_data_base.h
        misaxx/misa_default_cache.h
        misaxx/patterns/misa_dummy_pattern.cpp
        misaxx/patterns/misa_dummy_pattern.h
        misaxx/misa_manual_cache.h
        misaxx/accessors/misa_exported_attachments.cpp
        misaxx/accessors/misa_exported_attachments.h
        misaxx/accessors/misa_image_file.cpp
        misaxx/accessors/misa_image_file.h
        misaxx/accessors/misa_image_stack.cpp
        misaxx/accessors/misa_image_stack.h
        misaxx/misa_default_description_accessors.h
        misaxx/attachments/misa_location.cpp
        misaxx/attachments/misa_location.h
        misaxx/attachments/misa_unit.cpp
        misaxx/attachments/misa_unit.h
        misaxx/attachments/misa_unit_numeric.cpp
        misaxx/attachments/misa_unit_numeric.h
        misaxx/attachments/misa_quantity.cpp
        misaxx/attachments/misa_quantity.h
        misaxx/misa_serialization_id.cpp
        misaxx/attachments/misa_matrix.cpp
        misaxx/attachments/misa_matrix.h
        misaxx/misa_submodule_base.cpp
        misaxx/misa_submodule_base.h
        misaxx/runtime/misa_cli_base.cpp
        misaxx/runtime/misa_cli_base.h)

target_compile_features(misaxx-core PUBLIC cxx_std_17)
add_library(misaxx::core ALIAS misaxx-core)

generate_export_header(misaxx-core
        EXPORT_MACRO_NAME COIXX_API
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/misaxx/common.h
        )
target_include_directories(misaxx-core
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

# Include dependencies
target_link_libraries(misaxx-core PUBLIC
        Boost::filesystem
        Boost::regex
        Boost::program_options
        OpenMP::OpenMP_CXX
        nlohmann_json
        misaxx::misaxx-helpers
        misaxx::misaxx-coixx)

# Installation