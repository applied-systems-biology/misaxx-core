//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/misa_filesystem.h"
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
         * Internal function that imports a OS filesystem path into a MISA filesystem path
         * @param subdir
         * @param t_folder
         */
        void import_into(const boost::filesystem::path &subdir, const folder &t_folder) {

            std::cout << "[Filesystem] Importing folder " << subdir.string() << " into " << t_folder->internal_path().string() << std::endl;

            using namespace boost::filesystem;

            directory_iterator it { subdir };
            while(it != directory_iterator()) {
                path external_path = *it++;
                if(is_regular_file(external_path)) {
                    const auto f = t_folder->create<filesystem::file>(external_path.filename().string(), external_path);
                    std::cout << "[Filesystem] Importing file " << external_path.string() << " into " << f->internal_path().string() << std::endl;
                }
                else if(is_directory(external_path)) {
                    auto subfolder = t_folder->create<filesystem::folder>(external_path.filename().string(), external_path);
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
