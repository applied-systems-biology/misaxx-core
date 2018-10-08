//
// Created by rgerst on 05.10.18.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include "../vfs_folder.h"
#include "../../misa_filesystem.h"

namespace misaxx::filesystem::importers {


    /**
     * Empty importer used by parameter schema builder
     */
    struct empty_importer {

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<filesystem::vfs_folder>("imported");
            vfs.exported = std::make_shared<filesystem::vfs_folder>("exported");
            return vfs;
        }
    };
}
