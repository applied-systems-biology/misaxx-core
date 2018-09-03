//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include "misaxx/misa_filesystem.h"

namespace misaxx::filesystem::importers {

    /**
     * Imports a MISA++ filesystem from a JSON configuration file
     * The JSON file should have following format:
     *
     * \code{.json}
     * {
     *  "imported" : {
     *      "type" : "directory",
     *      "external-path" : "<optional external path>",
     *      "children" : {
     *          "file.txt" : {
     *              "type" : "file",
     *              "external-path" : "<optional external path>"
     *          }
     *      }
     *  },
     *  "exported" : {
     *  }
     * }
     * \endcode
     */
    struct json_importer {
        boost::filesystem::path json_path;

        /**
         * Internal function used for importing
         * @param t_json
         * @param t_folder
         */
        void import_folder(const nlohmann::json &t_json, filesystem::folder &t_folder) {

            if(t_json.find("external-path") != t_json.end()) {
                t_folder->custom_external = t_json["external-path"].get<std::string>();
            }

            if(t_json.find("children") != t_json.end()) {
                const nlohmann::json &children = t_json["children"];
                for(nlohmann::json::const_iterator kv = children.begin(); kv != children.end(); ++kv) {
                    const nlohmann::json &json_entry = kv.value();
                    if(json_entry["type"] == "file") {
                        file f = t_folder->create<vfs_file>(kv.key());
                        if(json_entry.find("external-path") != json_entry.end()) {
                            f->custom_external = json_entry["external-path"].get<std::string>();
                        }
                    }
                    else if(json_entry["type"] == "directory") {
                        folder f = t_folder->create<vfs_folder>(kv.key());
                        import_folder(json_entry, f);
                    }
                    else {
                        throw std::runtime_error("Unsupported filesystem entry type " + json_entry["type"].get<std::string>());
                    }
                    folder subfolder = t_folder->create<vfs_folder>(kv.key());

                }
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

            std::ifstream stream;
            stream.open(json_path.string());
            nlohmann::json json;
            stream >> json;

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