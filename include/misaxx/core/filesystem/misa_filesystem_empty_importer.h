//
// Created by rgerst on 05.10.18.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include <misaxx/core/filesystem/misa_filesystem_entry.h>
#include <misaxx/core/filesystem/misa_filesystem.h>

namespace misaxx {


    /**
     * Empty importer used by parameter schema builder
     */
    struct misa_filesystem_empty_importer {

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import();
    };
}
