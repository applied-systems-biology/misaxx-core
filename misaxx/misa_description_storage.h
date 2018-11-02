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
#include <cxxh/containers/drilldown_singleton_map.h>

namespace misaxx {
    /**
     * Metadata that is attached to filesystem entries.
     * For flexibility, the filesystem metadata holds the raw JSON data that can be interpreted as misa_metadata instances
     */
    struct misa_description_storage : public misa_serializeable {

        misa_description_storage() = default;

        template<typename Arg, typename... Args> explicit misa_description_storage(Arg &&arg, Args&&... args) {
            set(std::forward<Arg>(arg));
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
         * Loads from an imported JSON where all the contents in the JSON can be pattern information.
         * If a pattern or description are existing, the standard deserialization is used.
         * @param t_json
         */
        void from_imported_json(const nlohmann::json &t_json) {
            if(t_json.find("pattern") != t_json.end() || t_json.find("description") != t_json.end()) {
                from_json(t_json);
            }
            else {
                m_raw_pattern_json = t_json;
            }
        }

        void from_json(const nlohmann::json &t_json) override {
            {
                auto it = t_json.find("pattern");
                if(it != t_json.end()) {
                    m_raw_pattern_json = it.value();
                }
            }
            {
                auto it = t_json.find("description");
                if(it != t_json.end()) {
                    m_raw_description_json = it.value();
                }
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["pattern"] = m_raw_pattern_json; // Pass along the raw metadata. This is very important!
            t_json["description"] = m_raw_description_json;

            // Serialize any instances
            for(const auto &v : m_instances) {
                const auto *as_pattern = dynamic_cast<const misa_data_pattern_base*>(v.get());
                const auto *as_description = dynamic_cast<const misa_data_description*>(v.get());

                if(as_pattern) {
                    auto &j = t_json["pattern"];
                    v->to_json(j);
                    t_json["pattern"]["pattern-type"] = v->get_name();
                }
                else if(as_description) {
                    auto &j = t_json["description"];
                    v->to_json(j);
                    t_json["description"]["description-type"] = v->get_name();
                }
                else {
                    throw std::runtime_error("Cannot serialize unknown description type!");
                }
            }
        }

        std::string get_name() const override {
            return "misa-description-storage";
        }

        /**
         * Gets a description from this metadata instance. It is deserialized from the
         * internal JSON if needed.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> const Metadata &get() const {
            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                if(m_instances.find<Metadata>() == m_instances.end()) {
                    m_instances.access<Metadata>().from_json(m_raw_pattern_json);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if(m_instances.find<Metadata>() == m_instances.end()) {
                    m_instances.access<Metadata>().from_json(m_raw_description_json);
                }
            }
            else {
                static_assert(cxxh::types::template always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }

            return m_instances.at<Metadata>();
        }

        /**
        * Gets a description from this metadata instance. It is deserialized from the
        * internal JSON if needed.
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &get() {
            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                if(m_instances.find<Metadata>() == m_instances.end()) {
                    m_instances.access<Metadata>().from_json(m_raw_pattern_json);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if(m_instances.find<Metadata>() == m_instances.end()) {
                    m_instances.access<Metadata>().from_json(m_raw_description_json);
                }
            }
            else {
                static_assert(cxxh::types::template always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }

            return m_instances.at<Metadata>();
        }

        /**
         * Sets a description. Existing descriptions are overwritten
         * @tparam Metadata
         * @return
         */
        template <class Metadata> Metadata &set(Metadata t_description) {
            m_instances.insert(std::move(t_description));
            return m_instances.access<Metadata>();
        }

        /**
         * Sets a description. Existing descriptions are NOT overwritten.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> Metadata &access(Metadata t_description) {
            if(!m_instances.has<Metadata>())
                return set(std::move(t_description));
            else
                return m_instances.access<Metadata>();
        }

        /**
        * Sets a description. Existing descriptions are NOT overwritten.
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &access() {
            return m_instances.access<Metadata>();
        }

        /**
        * Sets a description. Existing descriptions are overwritten
        * @tparam Metadata
        * @return
        */
        template <class Metadata> bool has_description() {
            return m_instances.has<Metadata>();
        }

    private:

        /**
         * Raw JSON carried along from the settings to allow flexible importing (upgrading) of information during processing
         */
        nlohmann::json m_raw_pattern_json;

        /**
        * Raw JSON carried along from the settings to allow flexible importing (upgrading) of information during processing
        */
        nlohmann::json m_raw_description_json;

        /**
         * The drilldown implements all necessary methods of virtual access
         */
        mutable cxxh::containers::drilldown_singleton_map<misa_serializeable> m_instances;

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
