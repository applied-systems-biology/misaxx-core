//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/filesystem/virtual_filesystem.h"

namespace misaxx::filesystem::importers {

    /**
     * Imports a filesystem from an input folder.
     */
    struct folder_importer {
        boost::filesystem::path input_path;
        boost::filesystem::path output_path;

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

        virtual_filesystem import() {
            virtual_filesystem vfs;
            import_into(input_path, *vfs.root / "input");



            return vfs;
        }
    };
}
