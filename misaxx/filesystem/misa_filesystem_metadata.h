//
// Created by rgerst on 25.10.18.
//


#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pattxx/json/json_schema_builder.h>

namespace misaxx::filesystem {
    /**
     * Metadata that is attached to filesystem entries
     */
    struct misa_filesystem_metadata {

        /**
         * Raw JSON containing additional parameters for the importer
         */
        nlohmann::json data_parameters; // TODO: Will be later removed and replaced by proper JSON schema

        /**
         * The data type how the filesystem data is being interpreted
         */
        std::string data_type;

        /**
         * If true, this metadata is empty
         * @return
         */
        bool is_empty() {
            return data_type.empty();
        }
    };

    inline void from_json(const nlohmann::json &t_json, misa_filesystem_metadata &t_data) {
        t_data.data_type = t_json["data-type"];
        t_data.data_parameters = t_json["data-parameters"];
    }

    inline void to_json_schema(pattxx::json::json_schema_builder &t_builder, const pattxx::json::path_t &t_path, const misa_filesystem_metadata &t_data) {
        throw std::runtime_error("Not implemented!");
    }
}
