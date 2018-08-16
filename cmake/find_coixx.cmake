include(FetchContent)

FetchContent_Declare(
        coixx
        GIT_REPOSITORY https://asb-git.hki-jena.de/RGerst/coixx.git
        GIT_TAG origin/master
)

FetchContent_GetProperties(coixx)
if(NOT coixx_POPULATED)
    message("-- Fetching dependency coixx ...")
    FetchContent_Populate(coixx)
    add_subdirectory(${coixx_SOURCE_DIR} ${coixx_BINARY_DIR})
endif()