//
// Created by rgerst on 25.10.18.
//


#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pattxx/json/json_schema_builder.h>
#include <cxxh/containers/dynamic_singleton_map.h>
#include "../misa_metadata.h"

namespace misaxx {
    /**
     * Metadata that is attached to filesystem entries.
     * For flexibility, the filesystem metadata holds the raw JSON file that can be interpreted as misa_metadata instances
     */
    struct misa_filesystem_metadata : public misa_metadata {

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

        void from_json(const nlohmann::json &t_json) override {
            data_type = t_json["data-type"];
            m_raw_metadata = t_json;
        }

        nlohmann::json to_json() const override {

        }

        std::string get_name() const override {
            return "filesystem-metadata";
        }

    private:
        nlohmann::json m_raw_metadata;
        cxxh::containers::dynamic_singleton_map<misa_metadata> m_metadata_instances;

    };

    inline void to_json_schema(pattxx::json::json_schema_builder &t_builder, const pattxx::json::path_t &t_path, const misa_filesystem_metadata &t_data) {
        throw std::runtime_error("Not implemented!");
    }
}
