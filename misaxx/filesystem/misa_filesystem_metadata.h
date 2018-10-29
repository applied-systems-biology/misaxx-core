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
            throw std::runtime_error("Not implemented!");
        }

        std::string get_name() const override {
            return "filesystem-metadata";
        }

        /**
         * Gets a description from this metadata instance. It is deserialized from the
         * internal JSON if needed.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> const Metadata &get_description() {
            if(m_metadata_instances.find<Metadata>() != m_metadata_instances.end()) {
                m_metadata_instances.access<Metadata>().from_json(m_raw_metadata);
            }

            return m_metadata_instances.at<Metadata>();
        }

    private:
        nlohmann::json m_raw_metadata;
        mutable cxxh::containers::dynamic_singleton_map<misa_metadata> m_metadata_instances;

    };

    void to_json(nlohmann::json& j, const misa_filesystem_metadata& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, misa_filesystem_metadata& p) {
        p.from_json(j);
    }

    inline void to_json_schema(pattxx::json::json_schema_builder &t_builder, const pattxx::json::path_t &t_path, const misa_filesystem_metadata &t_data) {
        throw std::runtime_error("Not implemented!");
    }
}
