//
// Created by rgerst on 05.10.18.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include <misaxx/filesystem/misa_filesystem_entry.h>
#include <misaxx/filesystem/misa_filesystem.h>

namespace misaxx {


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
            vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type::imported);
            vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type::exported);
            return vfs;
        }
    };
}
