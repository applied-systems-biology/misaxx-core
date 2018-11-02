//
// Created by rgerst on 25.10.18.
//


#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pattxx/json/json_schema_builder.h>
#include <cxxh/containers/dynamic_singleton_map.h>
#include <misaxx/misa_data_pattern.h>
#include <misaxx/misa_data_description.h>
#include <misaxx/misa_serializeable.h>
#include <cxxh/types/static_helpers.h>

namespace misaxx {
    /**
     * Metadata that is attached to filesystem entries.
     * For flexibility, the filesystem metadata holds the raw JSON data that can be interpreted as misa_metadata instances
     */
    struct misa_description_storage : public misa_serializeable {

        /**
         * The data type how the filesystem data is being interpreted
         */
        std::string data_type;

        misa_description_storage() = default;

        template<typename Arg, typename... Args> explicit misa_description_storage(Arg &&arg, Args&&... args) {
            describe(std::forward<Arg>(arg));
            if constexpr (sizeof...(Args) > 0) {
                misa_description_storage(std::forward<Args>(args)...);
            }
        }

        misa_description_storage(const misa_description_storage &t_source) {
            nlohmann::json json;
            t_source.to_json(json);
            from_json(json);
        }

        misa_description_storage(misa_description_storage && t_source) = default;

        /**
         * Clones this metadata
         * @return
         */
        std::shared_ptr<misa_description_storage> clone() const {
            return std::make_shared<misa_description_storage>(*this);
        }

        /**
         * If true, this metadata is empty
         * @return
         */
        bool is_empty() {
            return data_type.empty();
        }

        /**
         * Loads from an imported JSON where all the data is assumed to be pattern metadata
         * @param t_json
         */
        void from_imported_json(const nlohmann::json &t_json) {
            data_type = t_json["data-type"];
            m_raw_pattern_metadata = t_json;
        }

        void from_json(const nlohmann::json &t_json) override {
            data_type = t_json["data-type"];
            {
                auto it = t_json.find("patterns");
                if(it != t_json.end()) {
                    m_raw_pattern_metadata = it.value();
                }
            }
            {
                auto it = t_json.find("descriptions");
                if(it != t_json.end()) {
                    m_raw_description_metadata = it.value();
                }
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["patterns"] = m_raw_pattern_metadata; // Pass along the raw metadata. This is very important!
            t_json["descriptions"] = m_raw_description_metadata;

            // Serialize any instances
            for(const auto &kv : m_metadata_instances) {
                auto &j = t_json[kv.second->get_name()]; // Patterns and descriptions have special overrides to do this
                kv.second->to_json(j);
            }
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
        template <class Metadata> const Metadata &get_description() const {
            if constexpr (std::is_base_of<misa_pattern_base, Metadata>::value) {
                if(m_metadata_instances.find<Metadata>() == m_metadata_instances.end()) {
                    m_metadata_instances.access<Metadata>().from_json(m_raw_pattern_metadata);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if(m_metadata_instances.find<Metadata>() == m_metadata_instances.end()) {
                    m_metadata_instances.access<Metadata>().from_json(m_raw_description_metadata);
                }
            }
            else {
                static_assert(cxxh::types::template always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }

            return m_metadata_instances.at<Metadata>();
        }

        /**
        * Gets a description from this metadata instance. It is deserialized from the
        * internal JSON if needed.
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &get_description() {
            if constexpr (std::is_base_of<misa_pattern_base, Metadata>::value) {
                if(m_metadata_instances.find<Metadata>() == m_metadata_instances.end()) {
                    m_metadata_instances.access<Metadata>().from_json(m_raw_pattern_metadata);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if(m_metadata_instances.find<Metadata>() == m_metadata_instances.end()) {
                    m_metadata_instances.access<Metadata>().from_json(m_raw_description_metadata);
                }
            }
            else {
                static_assert(cxxh::types::template always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }

            return m_metadata_instances.at<Metadata>();
        }

        /**
         * Sets a description.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> Metadata &describe(Metadata t_description) {
            m_metadata_instances.access<Metadata>() = std::move(t_description);
            return m_metadata_instances.access<Metadata>();
        }

    private:
        nlohmann::json m_raw_pattern_metadata;
        nlohmann::json m_raw_description_metadata;
        mutable cxxh::containers::dynamic_singleton_map<misa_serializeable> m_metadata_instances;

    };

    void to_json(nlohmann::json& j, const misa_description_storage& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_description_storage& p) {
        p.from_json(j);
    }

    inline void to_json_schema(pattxx::json::json_schema_builder &t_builder, const pattxx::json::path_t &t_path, const misa_description_storage &t_data) {
        throw std::runtime_error("Not implemented!");
    }
}
