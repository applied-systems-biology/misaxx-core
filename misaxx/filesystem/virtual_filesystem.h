//
// Created by rgerst on 08.08.18.
//


#pragma once
#include <unordered_map>
#include "vfs_folder.h"

namespace misaxx::filesystem {

    /**
     * A virtual file system (VFS) that allows flexible interaction between modules and between other pipelines.
     * This is the root of the filesystem that contains
     */
    struct virtual_filesystem {
        folder root;

        virtual_filesystem() : root(std::make_shared<vfs_folder>("")) {
            update_root();
        }

        void update_root() {
            root->parent = std::weak_ptr<vfs_entry>();
            root->self = root;
        }
    };
}
