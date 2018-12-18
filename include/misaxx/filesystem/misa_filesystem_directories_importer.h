//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/filesystem/misa_filesystem_entry.h>
#include <iostream>

namespace misaxx {

    /**
     * Imports a filesystem from an input folder.
     * The "imported" directory maps to the input path.
     * The "exported" directory maps to the output path.
     */
    struct misa_filesystem_directories_importer {
        boost::filesystem::path input_path;
        boost::filesystem::path output_path;

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import();

    private:

        void discoverImporterEntry(const filesystem::entry &t_entry);

    };
}
