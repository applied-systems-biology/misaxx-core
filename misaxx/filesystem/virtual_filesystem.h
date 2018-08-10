//
// Created by rgerst on 08.08.18.
//


#pragma once
#include <unordered_map>
#include "vfs_folder.h"

namespace misaxx::filesystem {

    /**
     * A virtual file system (VFS) that allows flexible interaction between modules and between other pipelines.
     */
    struct virtual_filesystem : public vfs_folder {
        using vfs_folder::vfs_folder;
    };
}
