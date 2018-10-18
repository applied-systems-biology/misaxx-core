//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misa_filesystem.h"
#include "misa_filesystem_entry.h"
#include <iostream>

namespace misaxx::filesystem::importers {

    /**
     * Imports a filesystem from an input folder.
     * The "imported" directory maps to the input path.
     * The "exported" directory maps to the output path.
     */
    struct directories_importer {
        boost::filesystem::path input_path;
        boost::filesystem::path output_path;

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<misa_filesystem_entry>("imported", input_path);
            vfs.exported = std::make_shared<misa_filesystem_entry>("exported", output_path);
            return vfs;
        }
    };
}
