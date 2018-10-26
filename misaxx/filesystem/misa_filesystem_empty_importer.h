//
// Created by rgerst on 05.10.18.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include "misa_filesystem_entry.h"
#include "misa_filesystem.h"

namespace misaxx::filesystem::importers {


    /**
     * Empty importer used by parameter schema builder
     */
    struct misa_filesystem_empty_importer {

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<filesystem::misa_filesystem_entry>("imported");
            vfs.exported = std::make_shared<filesystem::misa_filesystem_entry>("exported");
            return vfs;
        }
    };
}
