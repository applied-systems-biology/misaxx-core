//
// Created by rgerst on 09.08.18.
//


#pragma once

#include "vfs_entry.h"

namespace misaxx::filesystem {

    /**
     * File in the MISA++ virtual file system
     */
    class vfs_file : public vfs_entry {
    public:
        using vfs_entry::vfs_entry;

        bool empty() const override {
            return false;
        }

        bool is_file() const override {
            return true;
        }

        bool is_folder() const override {
            return false;
        }
    };

    using file = std::shared_ptr<vfs_file>;
    using const_file = std::shared_ptr<const vfs_file>;
}
