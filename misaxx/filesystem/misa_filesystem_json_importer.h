//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/filesystem/misa_filesystem_entry.h>
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
        void import_entry(const nlohmann::json &t_json, const filesystem::entry &t_entry);

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import();
    };
}