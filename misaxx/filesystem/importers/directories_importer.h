//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/misa_filesystem.h"

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
         * Internal function that imports a OS filesystem path into a MISA filesystem path
         * @param subdir
         * @param t_folder
         */
        void import_into(boost::filesystem::path &subdir, folder t_folder) {

            using namespace boost::filesystem;

            directory_iterator it { subdir };
            while(it != directory_iterator()) {
                path external_path = *it++;
                if(is_regular_file(external_path)) {
                    t_folder->create<vfs_file>(external_path.filename().string(), external_path);
                }
                else if(is_directory(external_path)) {
                    auto subfolder = t_folder->create<vfs_folder>(external_path.filename().string(), external_path);
                    import_into(external_path, subfolder);
                }
            }
        }

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<filesystem::vfs_folder>("imported", input_path);
            vfs.exported = std::make_shared<filesystem::vfs_folder>("exported", output_path);
            import_into(input_path, vfs.imported);
            return vfs;
        }
    };
}
