//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_file.h"
#include <fstream>

namespace misaxx {
    /**
     * misa_file with additional operators for easier serialization of JSON data
     */
    struct misa_json_file : public misa_file {
        using misa_file::misa_file;

        void load (const nlohmann::json &json) {
            std::ofstream sw;
            sw.open(path.string());
            sw << json;
        }

        void save (nlohmann::json &json) {
            std::ifstream sr;
            sr.open(path.string());
            sr >> json;
        }
    };
}
