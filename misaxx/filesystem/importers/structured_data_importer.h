//
// Created by rgerst on 12.10.18.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include "misaxx/misa_filesystem.h"

namespace misaxx::filesystem::importers {

    /**
     * Similar to json_importer, but folders with a non-empty "data-type" property are additionally
     * imported from filesystem.
     */
    struct structured_data_importer {

        nlohmann::json input_json;
        boost::filesystem::path json_path;

        /**
         * Internal function that imports a OS filesystem path into a MISA filesystem path
         * @param subdir
         * @param t_folder
         */
        void import_into(const boost::filesystem::path &subdir, const folder &t_folder) {

            using namespace boost::filesystem;

            directory_iterator it { subdir };
            while(it != directory_iterator()) {
                path external_path = *it++;
                if(is_regular_file(external_path)) {
                    t_folder->create<filesystem::file>(external_path.filename().string(), external_path);
                }
                else if(is_directory(external_path)) {
                    auto subfolder = t_folder->create<filesystem::folder>(external_path.filename().string(), external_path);
                    import_into(external_path, subfolder);
                }
            }
        }

        /**
         * Internal function used for importing
         * @param t_json
         * @param t_folder
         */
        void import_folder(const nlohmann::json &t_json, const filesystem::folder &t_folder) {

            if(t_json.find("external-path") != t_json.end()) {
                t_folder->custom_external = t_json["external-path"].get<std::string>();
            }

            if(t_json.find("children") != t_json.end() && !t_json["children"].empty()) {
                const nlohmann::json &children = t_json["children"];
                for(nlohmann::json::const_iterator kv = children.begin(); kv != children.end(); ++kv) {
                    const nlohmann::json &json_entry = kv.value();
                    if(json_entry["type"] == "file") {
                        file f = t_folder->create<filesystem::file>(kv.key());
                        if(json_entry.find("external-path") != json_entry.end()) {
                            f->custom_external = json_entry["external-path"].get<std::string>();
                        }
                    }
                    else if(json_entry["type"] == "folder") {
                        folder f = t_folder->create<folder>(kv.key());
                        import_folder(json_entry, f);
                    }
                    else {
                        throw std::runtime_error("Unsupported filesystem entry type " + json_entry["type"].get<std::string>());
                    }
                    folder subfolder = t_folder->create<folder>(kv.key());

                }
            }
            else {
                bool has_data_type  = t_json.find("data-type") != t_json.end() && t_json["data-type"].is_string() && t_json["data-type"].get<std::string>().empty();
                if(has_data_type)
                    import_into(t_folder->external_path(), t_folder);
            }
        }

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<filesystem::vfs_folder>("imported");
            vfs.exported = std::make_shared<filesystem::vfs_folder>("exported");

            nlohmann::json json;

            if(input_json.empty()) {
                std::ifstream stream;
                stream.open(json_path.string());
                stream >> json;
            }
            else {
                json = input_json;
            }

            if(json.find("imported") != json.end()) {
                import_folder(json["imported"], vfs.imported);
            }
            if(json.find("exported") != json.end()) {
                import_folder(json["exported"], vfs.exported);
            }

            return vfs;
        }
    };
}