//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include "misa_filesystem.h"
#include "misa_filesystem_entry.h"
#include <iostream>

namespace misaxx {

    /**
     * Imports a MISA++ filesystem from a JSON configuration file
     * The JSON data should have following format:
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
     *
     * Please note that this importer requires that all files are defined in the JSON
     * data. If the JSON data should only contain partially defined structures (such as
     * the existance of a file stack, but not all files), use structured_data_importer instead.
     */
    struct misa_filesystem_json_importer {

        nlohmann::json input_json;
        boost::filesystem::path json_path;

        /**
         * Internal function used for importing
         * @param t_json
         * @param t_entry
         */
        void import_entry(const nlohmann::json &t_json, const filesystem::entry &t_entry) {

            if(t_json.find("external-path") != t_json.end()) {
                t_entry->custom_external = t_json["external-path"].get<std::string>();
                std::cout << "[Filesystem][json-importer] Importing entry " << t_entry->custom_external.string() << " into " << t_entry->internal_path().string() << std::endl;
            }
            else {
                std::cout << "[Filesystem][json-importer] Importing entry " << t_entry->internal_path().string() << std::endl;
            }

            // Load the metadata from JSON or file if applicable
            // File metadata is preferred
            if(t_entry->has_external_path() && boost::filesystem::is_regular_file(t_entry->external_path() / "metadata.json")) {
                std::cout << "[Filesystem][json-importer] Importing metadata from file " << (t_entry->external_path() / "metadata.json").string() << std::endl;
                nlohmann::json json;
                std::ifstream stream;
                stream.open((t_entry->external_path() / "metadata.json").string());
                stream >> json;
                t_entry->metadata.from_json(json);
            }
            else if(t_json.find("data-metadata") != t_json.end()) {
                std::cout << "[Filesystem][json-importer] Importing metadata from JSON" << std::endl;
                t_entry->metadata.from_json(t_json["data-metadata"]);
            }

            if(t_json.find("children") != t_json.end()) {
                const nlohmann::json &children = t_json["children"];
                for(nlohmann::json::const_iterator kv = children.begin(); kv != children.end(); ++kv) {
                    const nlohmann::json &json_entry = kv.value();
                    filesystem::entry f = t_entry->create(kv.key());
                    import_entry(json_entry, f);
                }
            }
        }

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import() {
            misa_filesystem vfs;
            vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type::imported);
            vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type::exported);

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
                import_entry(json["imported"], vfs.imported);
            }
            if(json.find("exported") != json.end()) {
                import_entry(json["exported"], vfs.exported);
            }

            return vfs;
        }
    };
}