//
// Created by rgerst on 08.08.18.
//


#pragma once
#include <unordered_map>
#include "misa_folder.h"

namespace misaxx::filesystem {

    /**
     * A virtual filesystem that allows to generate the required file structure without changing any file.
     * There are multiple importers available that can import a filesystem from a directory or
     * from a JSON configuration.
     */
    struct misa_filesystem : public misa_folder {
        using misa_folder::misa_folder;
    };
}
