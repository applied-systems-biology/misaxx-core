//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misa_module_definition_base.h"
#include "misa_module_data.h"
#include "filesystem/vfs_file.h"

namespace misaxx {
    struct misa_file : public misa_module_data {
        boost::filesystem::path path;

        using misa_module_data::misa_module_data;

        /**
         * Initializes this MISA++ file from a VFS file
         * @param t_module
         * @param t_file
         * @param force
         */
        void init(misa_module_definition_base &t_module, const filesystem::file &t_file, bool force = false) {
            if(has_value && !force)
                return;
            path = t_file->external_path();
            has_value = true;
        }

        /**
         * Initializes this MISA++ file from an existing file. Creates a file /exported/<name>
         * @param t_module
         * @param t_reference_file
         * @param force
         */
        void init(misa_module_definition_base &t_module, const misa_file &t_reference_file, bool force = false) {
            if(has_value && !force)
                return;
            filesystem::file f = t_module.filesystem.exported->create<filesystem::vfs_file>(t_reference_file.path.filename().string());
            path = f->external_path();
            has_value = true;
        }
    };
}
