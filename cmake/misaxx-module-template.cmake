#
# This file contains additional functions that make it easier to create MISA++ modules
#
include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Adds a default executable '<library>-bin' for the MISA++ library
# The executable creates an installable target
function(misaxx_add_default_executable library)
    message("Adding default executable for ${library}")
    # If necessary, create the default executable path
    if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
        message("Using existing main.cpp")
    else()
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/)

        # Auto-gen the name for the module
        string(REPLACE "-" "_" module_name "${library}")
        file(WRITE ${CMAKE_SOURCE_DIR}/src/main.cpp "\#include <${module_name}/${module_name}_module.h>\n\
\#include <misaxx/runtime/misa_cli.h>\n\
\n\
using namespace misaxx;\n\
using namespace ${module_name};\n\
\n\
int main(int argc, const char** argv) {\n\
    misa_cli<misa_multiobject_root<${module_name}_module>> cli(\"${library}\");\n\
    return cli.prepare_and_run(argc, argv);\n\
}")
    endif()

    # Module executable
    add_executable("${library}-bin" src/main.cpp)
    target_link_libraries("${library}-bin" ${library})
    set_target_properties("${library}-bin" PROPERTIES OUTPUT_NAME "${library}")

    # Create install target for the executable
    install(TARGETS "${library}-bin" DESTINATION bin)
endfunction()

# Creates a shared library from the input library
function(misaxx_make_shared namespace library)
    # Option for shared library
    option(BUILD_SHARED_LIBS "Build shared library" ON)

    # If necessary, create the *.in file for library configuration
    if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in)
        message("Using existing library configuration file")
    else()
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/)
        file(WRITE ${CMAKE_SOURCE_DIR}/cmake/${library}-config.cmake.in "\@PACKAGE_INIT\@\n\
find_package(misaxx-core REQUIRED)\n\
if(NOT TARGET ${library})\n\
include(${CMAKE_CURRENT_LIST_DIR}/${library}-targets.cmake)\n\
endif()")
    endif()

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