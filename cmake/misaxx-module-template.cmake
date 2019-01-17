#
# This file contains additional functions that make it easier to create MISA++ modules
#
include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Adds a default executable '<library>-bin' for the MISA++ library
# The executable creates an installable target
function(misaxx_add_default_executable library)
    message("-- Adding default executable for ${library}")
    # If necessary, create the default executable path
    if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
        message("--   Using existing main.cpp")
    else()
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/)

        # Auto-gen the name for the module
        string(REPLACE "-" "_" module_name "${library}")

        message("--   Creating ${CMAKE_SOURCE_DIR}/src/main.cpp based on module name ${module_name}")
        file(WRITE ${CMAKE_SOURCE_DIR}/src/main.cpp "\#include <${library}/${module_name}_module.h>\n\
\#include <misaxx/runtime/misa_cli.h>\n\
\n\
using namespace misaxx;\n\
using namespace ${module_name};\n\
\n\
int main(int argc, const char** argv) {\n\
    misa_cli<misa_multiobject_root<${module_name}_module>> cli(\"${library}\");\n\
    return cli.prepare_and_run(argc, argv);\n\
}")
        message(WARNING "Please make sure that ${CMAKE_SOURCE_DIR}/src/main.cpp is correct")
    endif()

    # Module executable
    add_executable("${library}-bin" src/main.cpp)
    target_link_libraries("${library}-bin" ${library})
    set_target_properties("${library}-bin" PROPERTIES OUTPUT_NAME "${library}")

    # Create install target for the executable
    install(TARGETS "${library}-bin" DESTINATION bin)

    # Create and install the module link, so external programs can find this executable
    message("--   A module link JSON will be created for this executable")
    if(WIN32)
        set(MISA_MODULE_LINK_OPERATING_SYSTEM Windows)
        set(MISA_MODULE_LINK_EXECUTABLE_PATH ${CMAKE_INSTALL_PREFIX}/bin/${library}.exe)
    else()
        # Assume Linux here for now
        set(MISA_MODULE_LINK_OPERATING_SYSTEM Linux)
        set(MISA_MODULE_LINK_EXECUTABLE_PATH ${CMAKE_INSTALL_PREFIX}/bin/${library})
    endif()
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(MISA_MODULE_LINK_ARCHITECTURE x64)
    else()
        # Assume x32 here for now
        set(MISA_MODULE_LINK_ARCHITECTURE x32)
    endif()
    file(WRITE ${CMAKE_BINARY_DIR}/misa-module-link.json "{\n\
    \"operating-system\" : \"${MISA_MODULE_LINK_OPERATING_SYSTEM}\",\n\
    \"architecture\" : \"${MISA_MODULE_LINK_ARCHITECTURE}\",\n\
    \"executable-path\" : \"${MISA_MODULE_LINK_EXECUTABLE_PATH}\"\n\
}")
    install(FILES ${CMAKE_BINARY_DIR}/misa-module-link.json
            RENAME ${library}-${PROJECT_VERSION}-${MISA_MODULE_LINK_OPERATING_SYSTEM}-${MISA_MODULE_LINK_ARCHITECTURE}.json
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/misaxx/modules)

endfunction()

# Creates a shared library from the input library
function(misaxx_make_shared namespace library)

    message("-- ${library} is configured to be a shared library ${namespace}::${library}")

    # Option for shared library
    option(BUILD_SHARED_LIBS "Build shared library" ON)

    # If necessary, create the *.in file for library configuration
    if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in)
        message("--   Using existing library configuration file ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in")
    else()
        message("--   Creating configuration file ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in")
        message(WARNING "Please make sure that the package dependencies in ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in are consistent with the dependencies in CMakeLists.txt")
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/)
        file(WRITE ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in "\@PACKAGE_INIT\@\n\n\
find_package(misaxx-core REQUIRED)\n\n\
if(NOT TARGET ${library})\n\
include(\${CMAKE_CURRENT_LIST_DIR}/${library}-targets.cmake)\n\
endif()")
    endif()

    # If necessary, create a header that contains the module info
    if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/module_info.h.in)
        message("--   Module info header template already exists at ${CMAKE_SOURCE_DIR}/cmake/module_info.h.in")
    else()
        message("--   Creating module info header ${CMAKE_SOURCE_DIR}/cmake/module_info.h.in")
        message(WARNING "Please make sure that you include the dependencies in ${CMAKE_SOURCE_DIR}/cmake/module_info.h.in")
        # Auto-gen the name for the module
        string(REPLACE "-" "_" module_name "${library}")

        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/)
        file(WRITE ${CMAKE_SOURCE_DIR}/cmake/module_info.h.in "#pragma once\n\
#include <misaxx/misa_mutable_module_info.h>\n\
#include <misaxx/module_info.h>\n\
\n\
namespace ${module_name} {\n\
    inline misaxx::misa_module_info module_info() {\n\
        misaxx::misa_mutable_module_info info;\n\
        info.set_name(\"@PROJECT_NAME@\");\n\
        info.set_version(\"@PROJECT_VERSION@\");\n\
        info.set_description(\"@PROJECT_DESCRIPTION@\");\n\
        \n\
        info.add_dependency(misaxx::module_info());\n\
        // TODO: Add dependencies via info.add_dependency()\n\
        return info;
    }\n\
}")
    endif()

    configure_file(${CMAKE_SOURCE_DIR}/cmake/module_info.h.in
            ${CMAKE_BINARY_DIR}/include/${library}/module_info.h)
    target_sources(${library} PRIVATE ${CMAKE_BINARY_DIR}/include/${library}/module_info.h)

    # Internal alias
    add_library("${namespace}::${library}" ALIAS ${library})

    # Setup include directories
    string(REPLACE "-" "_" module_name "${library}")
    string(TOUPPER ${module_name} module_name_upper)
    generate_export_header(${library}
            EXPORT_MACRO_NAME "${module_name_upper}_API"
            EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/${library}/common.h
            )
    target_include_directories(${library}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            )

    # Install targets
    message("--   Creating default install operations")
    set_target_properties(${library} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            )
    install(TARGETS ${library}
            EXPORT "${library}-targets"
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${LIBLEGACY_INCLUDE_DIRS}
            )
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/${library}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    install(DIRECTORY ${CMAKE_BINARY_DIR}/include/${library}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    install(EXPORT ${library}-targets
            FILE ${library}-targets.cmake
            NAMESPACE "${namespace}::"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library}
            )
    configure_package_config_file(
            ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in
            ${CMAKE_BINARY_DIR}/cmake/${library}-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library}
    )
    write_basic_package_version_file(
            ${CMAKE_BINARY_DIR}/cmake/${library}-config-version.cmake
            VERSION ${${library}_VERSION}
            COMPATIBILITY AnyNewerVersion
    )
    install(
            FILES
            ${CMAKE_BINARY_DIR}/cmake/${library}-config.cmake
            ${CMAKE_BINARY_DIR}/cmake/${library}-config-version.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library}
    )
endfunction()

# Adds additional compiler warnings
function(misaxx_add_compiler_warnings library)
    # Additional warnings
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${library} PRIVATE -Wredundant-decls
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
endfunction()