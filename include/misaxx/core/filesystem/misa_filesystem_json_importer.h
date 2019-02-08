//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <misaxx/core/filesystem/misa_filesystem.h>
#include <misaxx/core/filesystem/misa_filesystem_entry.h>
#include <iostream>

namespace misaxx {

    /**
     * Imports a MISA++ filesystem from JSON data
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