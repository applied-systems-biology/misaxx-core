//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_unsafe_file.h"
#include <fstream>

namespace misaxx {
    /**
     * misa_file with additional operators for easier serialization of JSON data
     * Legacy class. Do not use.
     */
    struct [[deprecated]] misa_unsafe_json_file : public misa_unsafe_file {

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-json-file";

        using misa_unsafe_file::misa_unsafe_file;

        void load (const nlohmann::json &json) {
            std::ofstream sw;
            sw.open(get().string());
            sw << json;
        }

        void save (nlohmann::json &json) {
            std::ifstream sr;
            sr.open(get().string());
            sr >> json;
        }
    };
}
