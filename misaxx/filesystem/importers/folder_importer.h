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

        void import_into(boost::filesystem::path &subdir, vfs_folder &folder) {

            using namespace boost::filesystem;

            directory_iterator it { subdir };
            while(it != directory_iterator()) {
                path external_path = *it++;
                if(is_regular_file(external_path)) {
                    folder.create<vfs_file>(external_path.filename().string(), external_path);
                }
                else if(is_directory(external_path)) {
                    auto &subfolder = folder.create<vfs_folder>(external_path.filename().string(), external_path);
                    import_into(external_path, subfolder);
                }
            }
        }

        std::shared_ptr<virtual_filesystem> import() {
            std::shared_ptr<virtual_filesystem> result = std::make_shared<virtual_filesystem>("");
            import_into(input_path, *result);

            return result;
        }
    };
}
