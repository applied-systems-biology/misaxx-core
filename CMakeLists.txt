cmake_minimum_required(VERSION 3.11)
project(misaxx-core LANGUAGES CXX VERSION 0.1.0 DESCRIPTION "MISA++ Core")

include(FetchContent)
include(FeatureSummary)
include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Build options
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(WITH_CONAN "Build with Conan support if available" ON)

# Conan integration
set(CAN_USE_CONAN OFF)
if(WITH_CONAN AND EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    set(CAN_USE_CONAN ON)
    message("-- Enabling building with Conan package manager")
else()
    message("-- Building without Conan")
endif()

# Dependencies
if(CAN_USE_CONAN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)    
    conan_basic_setup(TARGETS)
    find_package(OpenMP REQUIRED)
    feature_summary(WHAT ALL)
else()
    find_package(misaxx-helpers REQUIRED)
    find_package(misaxx-coixx REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_package(Boost 1.63 COMPONENTS REQUIRED filesystem regex program_options)
    find_package(OpenMP REQUIRED)
    feature_summary(WHAT ALL)
endif()

# Setting up the library
add_library(misaxx-core
        include/misaxx/patterns/misa_image_file_stack_pattern.h
        include/misaxx/patterns/misa_file_pattern.h
        include/misaxx/patterns/misa_image_file_pattern.h
        include/misaxx/patterns/misa_file_stack_pattern.h
        include/misaxx/misa_module_interface.h
        include/misaxx/misa_cached_data.h
        include/misaxx/misa_cache.h
        include/misaxx/misa_root_module_base.h
        include/misaxx/misa_module.h
        include/misaxx/misa_data_pattern_base.h
        include/misaxx/misa_data_description.h
        include/misaxx/misa_multiobject_root.h
        include/misaxx/descriptions/misa_file_stack_description.h
        include/misaxx/descriptions/misa_file_description.h
        include/misaxx/json/misa_json_schema_builder.h
        include/misaxx/json/misa_json_schema.h
        include/misaxx/json/misa_json_property.h
        include/misaxx/json/misa_json_property_base.h
        include/misaxx/caches/misa_image_stack_cache.h
        include/misaxx/caches/misa_image_file_cache.h
        include/misaxx/caches/misa_exported_attachments_cache.h
        include/misaxx/filesystem/misa_filesystem_json_importer.h
        include/misaxx/filesystem/misa_filesystem_entry.h
        include/misaxx/filesystem/misa_filesystem_directories_importer.h
        include/misaxx/filesystem/misa_filesystem_empty_importer.h
        include/misaxx/filesystem/misa_filesystem.h
        include/misaxx/misa_dispatch_blueprint.h
        include/misaxx/runtime/misa_runtime_base.h
        include/misaxx/runtime/misa_runtime.h
        include/misaxx/runtime/misa_cli.h
        include/misaxx/misa_serializeable.h
        include/misaxx/misa_description_storage.h
        include/misaxx/workers/misa_work_node.h
        include/misaxx/workers/misa_work_subtree_status.h
        include/misaxx/workers/misa_worker_status.h
        include/misaxx/misa_task.h
        include/misaxx/misa_functional_task.h
        include/misaxx/workers/misa_work_dependency_group.h
        include/misaxx/workers/misa_work_dependency_chain.h
        include/misaxx/workers/misa_work_dependency_segment.h
        include/misaxx/misa_dispatcher.h
        include/misaxx/misa_worker.h
        include/misaxx/misa_data_pattern.h
        src/misaxx/patterns/misa_image_file_stack_pattern.cpp
        src/misaxx/patterns/misa_file_pattern.cpp
        src/misaxx/patterns/misa_image_file_pattern.cpp
        src/misaxx/patterns/misa_file_stack_pattern.cpp
        src/misaxx/misa_module_interface.cpp
        src/misaxx/misa_cached_data.cpp
        src/misaxx/misa_cache.cpp
        src/misaxx/misa_root_module_base.cpp
        src/misaxx/misa_module.cpp
        src/misaxx/misa_data_pattern_base.cpp
        src/misaxx/misa_data_description.cpp
        src/misaxx/misa_multiobject_root.cpp
        src/misaxx/descriptions/misa_file_stack_description.cpp
        src/misaxx/descriptions/misa_file_description.cpp
        src/misaxx/json/misa_json_schema_builder.cpp
        src/misaxx/json/misa_json_schema.cpp
        src/misaxx/json/misa_json_property.cpp
        src/misaxx/json/misa_json_property_base.cpp
        src/misaxx/caches/misa_image_stack_cache.cpp
        src/misaxx/caches/misa_image_file_cache.cpp
        src/misaxx/caches/misa_exported_attachments_cache.cpp
        src/misaxx/filesystem/misa_filesystem_json_importer.cpp
        src/misaxx/filesystem/misa_filesystem_entry.cpp
        src/misaxx/filesystem/misa_filesystem_directories_importer.cpp
        src/misaxx/filesystem/misa_filesystem_empty_importer.cpp
        src/misaxx/filesystem/misa_filesystem.cpp
        src/misaxx/misa_dispatch_blueprint.cpp
        src/misaxx/runtime/misa_runtime_base.cpp
        src/misaxx/runtime/misa_runtime.cpp
        src/misaxx/runtime/misa_cli.cpp
        src/misaxx/misa_serializeable.cpp
        src/misaxx/misa_description_storage.cpp
        src/misaxx/workers/misa_work_node.cpp
        src/misaxx/workers/misa_work_subtree_status.cpp
        src/misaxx/workers/misa_worker_status.cpp
        src/misaxx/misa_task.cpp
        src/misaxx/workers/misa_work_dependency_group.cpp
        src/misaxx/workers/misa_work_dependency_chain.cpp
        src/misaxx/workers/misa_work_dependency_segment.cpp
        src/misaxx/misa_dispatcher.cpp
        src/misaxx/misa_worker.cpp
        src/misaxx/misa_data_pattern.cpp
        include/misaxx/misa_primitive.h
        include/misaxx/misa_serialization_id.h
        include/misaxx/json/misa_json_helper.h
        src/misaxx/misa_cached_data_base.cpp
        include/misaxx/misa_cached_data_base.h
        include/misaxx/misa_default_cache.h
        src/misaxx/patterns/misa_dummy_pattern.cpp
        include/misaxx/patterns/misa_dummy_pattern.h
        include/misaxx/misa_manual_cache.h
        src/misaxx/accessors/misa_exported_attachments.cpp
        include/misaxx/accessors/misa_exported_attachments.h
        src/misaxx/accessors/misa_image_file.cpp
        include/misaxx/accessors/misa_image_file.h
        src/misaxx/accessors/misa_image_stack.cpp
        include/misaxx/accessors/misa_image_stack.h
        include/misaxx/misa_default_description_accessors.h
        src/misaxx/attachments/misa_location.cpp
        include/misaxx/attachments/misa_location.h
        src/misaxx/attachments/misa_unit.cpp
        include/misaxx/attachments/misa_unit.h
        src/misaxx/attachments/misa_unit_numeric.cpp
        include/misaxx/attachments/misa_unit_numeric.h
        src/misaxx/attachments/misa_quantity.cpp
        include/misaxx/attachments/misa_quantity.h
        src/misaxx/misa_serialization_id.cpp
        src/misaxx/attachments/misa_matrix.cpp
        include/misaxx/attachments/misa_matrix.h
        src/misaxx/runtime/misa_cli_base.cpp
        include/misaxx/runtime/misa_cli_base.h 
        include/misaxx/attachments/misa_locatable.h 
        include/misaxx/attachments/misa_locatable_wrapper.h 
        include/misaxx/attachments/misa_labeled_object_location.h 
        src/misaxx/attachments/misa_labeled_object_location.cpp
        src/misaxx/misa_dispatcher_blueprint_list.cpp
        include/misaxx/misa_dispatcher_blueprint_list.h
        src/misaxx/misa_dispatcher_builder.cpp
        include/misaxx/misa_dispatcher_builder.h
        include/misaxx/misa_parameter_base.h
        include/misaxx/misa_parameter.h
        include/misaxx/workers/misa_work_tree_path.h
        src/misaxx/workers/misa_work_tree_path.cpp
        include/misaxx/workers/misa_work_tree_node_path.h
        src/misaxx/workers/misa_work_tree_node_path.cpp
        src/misaxx/workers/misa_work_tree_sample_path.cpp
        include/misaxx/workers/misa_work_tree_sample_path.h
        src/misaxx/workers/misa_work_tree_algorithm_path.cpp
        include/misaxx/workers/misa_work_tree_algorithm_path.h
        src/misaxx/misa_parameter_builder.cpp
        include/misaxx/misa_parameter_builder.h
        include/misaxx/runtime/misa_runtime_properties.h
        include/misaxx/runtime/misa_cache_registry.h
        include/misaxx/runtime/misa_parameter_registry.h
        src/misaxx/runtime/misa_runtime_properties.cpp
        src/misaxx/runtime/misa_cache_registry.cpp
        src/misaxx/runtime/misa_parameter_registry.cpp
        src/misa_module_info.cpp include/misaxx/misa_module_info.h
        src/misaxx/misa_mutable_module_info.cpp
        include/misaxx/misa_mutable_module_info.h
        )

# Module info header
configure_file(${CMAKE_SOURCE_DIR}/cmake/module_info.h.in
        ${CMAKE_BINARY_DIR}/include/misaxx/module_info.h)
target_sources(misaxx-core PRIVATE ${CMAKE_BINARY_DIR}/include/misaxx/module_info.h)

# For installation
target_compile_features(misaxx-core PUBLIC cxx_std_17)
add_library(misaxx::misaxx-core ALIAS misaxx-core)

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

# Additional warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(misaxx-core PRIVATE -Wredundant-decls
            -Wcast-align
            -Wmissing-declarations
            -Wmissing-include-dirs
            -Wswitch-enum
            -Wswitch-default
            -Wextra
            -Wall
            -Winvalid-pch
            -Wredundant-decls)
endif()

# Include dependencies
if(CAN_USE_CONAN)
    target_link_libraries(misaxx-core PUBLIC OpenMP::OpenMP_CXX CONAN_PKG::boost CONAN_PKG::jsonformoderncpp CONAN_PKG::misaxx-coixx CONAN_PKG::misaxx-helpers)
else()
    target_link_libraries(misaxx-core PUBLIC
        Boost::filesystem
        Boost::regex
        Boost::program_options
        OpenMP::OpenMP_CXX
        nlohmann_json
        misaxx::misaxx-helpers
        misaxx::misaxx-coixx)
endif()

# Installation
set_target_properties(misaxx-core PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
install(TARGETS misaxx-core
        EXPORT misaxx-core-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${LIBLEGACY_INCLUDE_DIRS}
        )
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/misaxx
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/misaxx
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
install(EXPORT misaxx-core-targets
        FILE misaxx-core-targets.cmake
        NAMESPACE misaxx::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/misaxx-core
        )
configure_package_config_file(
        ${CMAKE_SOURCE_DIR}/cmake/misaxx-core-config.cmake.in
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/misaxx-core
)
write_basic_package_version_file(
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config-version.cmake
        VERSION ${misaxx-core_VERSION}
        COMPATIBILITY AnyNewerVersion
)
install(
        FILES
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/misaxx-core-config-version.cmake
        ${CMAKE_SOURCE_DIR}/cmake/misaxx-module-template.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/misaxx-core
)
