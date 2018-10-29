//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misa_filesystem.h"
#include "misa_filesystem_entry.h"
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
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type ::imported, input_path);
            discoverImporterEntry(vfs.imported);
            vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type ::exported, output_path);
            return vfs;
        }

    private:

        void discoverImporterEntry(const filesystem::entry &t_entry) {
            std::cout << "[Filesystem][directories-importer] Importing entry " << t_entry->internal_path().string() << " @ " << t_entry->external_path().string() << std::endl;
            auto metadata_file = t_entry->external_path() / "metadata.json";
            if(boost::filesystem::is_regular_file(metadata_file)) {
                std::cout << "[Filesystem][directories-importer] Importing metadata from file " << metadata_file.string() << std::endl;
                nlohmann::json json;
                std::ifstream stream;
                stream.open(metadata_file.string());
                stream >> json;
                t_entry->metadata.from_json(json);
            }

            for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_entry->external_path()))) {
                if(boost::filesystem::is_directory(entry)) {
                    filesystem::entry e = t_entry->create(entry.path().filename().string());
                    discoverImporterEntry(e);
                }
            }
        }

    };
}
