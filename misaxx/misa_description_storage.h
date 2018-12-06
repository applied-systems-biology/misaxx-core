//
// Created by rgerst on 25.10.18.
//


#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <misaxx/misa_data_pattern_base.h>
#include <misaxx/misa_data_description.h>
#include <misaxx/misa_serializeable.h>
#include <cxxh/type_traits.h>
#include <cxxh/drilldown_singleton_map.h>

namespace misaxx {
    /**
     * Metadata that is attached to filesystem entries.
     * For flexibility, the filesystem metadata holds the raw JSON data that can be interpreted as misa_metadata instances
     */
    struct misa_description_storage : public misa_serializeable {

        misa_description_storage() = default;

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
                m_has_pattern = !m_raw_pattern_json.empty();
            }
        }

        void from_json(const nlohmann::json &t_json) override {
            {
                auto it = t_json.find("pattern");
                if(it != t_json.end()) {
                    m_raw_pattern_json = it.value();
                    m_has_pattern = true;
                }
            }
            {
                auto it = t_json.find("description");
                if(it != t_json.end()) {
                    m_raw_description_json = it.value();
                    m_has_description = true;
                }
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            t_json["pattern"] = m_raw_pattern_json; // Pass along the raw metadata. This is very important!
            t_json["description"] = m_raw_description_json;

            // Serialize any instances
            for(const auto &v : m_instances) {
                const auto *as_pattern = dynamic_cast<const misa_data_pattern_base*>(v.get());
                const auto *as_description = dynamic_cast<const misa_data_description*>(v.get());

                if(as_pattern) {
                    auto &j = t_json["pattern"];
                    v->to_json(j);
                    t_json["pattern"]["pattern-type"] = v->get_serialization_id();
                }
                else if(as_description) {
                    auto &j = t_json["description"];
                    v->to_json(j);
                    t_json["description"]["description-type"] = v->get_serialization_id();
                }
                else {
                    throw std::runtime_error("Cannot serialize unknown description type!");
                }
            }
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            if(has_pattern()) {
                t_schema.resolve("pattern", "pattern-type").define(get<misa_data_pattern_base>().get_serialization_id());
                get<misa_data_pattern_base>().to_json_schema(t_schema.resolve("pattern"));
            }
            if(has_description()) {
                t_schema.resolve("description", "description-type").define(get<misa_data_description>().get_serialization_id());
                get<misa_data_description>().to_json_schema(t_schema.resolve("description"));
            }
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "description-storage");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return { get_serialization_id() };
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
                static_assert(cxxh::template always_false<Metadata>::value, "Only patterns and descriptions are supported!");
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
                static_assert(cxxh::template always_false<Metadata>::value, "Only patterns and descriptions are supported!");
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

            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                m_has_pattern = true;
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                m_has_description = true;
            }

            return m_instances.access<Metadata>();
        }

        /**
         * Sets a description. Existing descriptions are NOT overwritten.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> Metadata &access(Metadata t_description) {

            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                m_has_pattern = true;
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                m_has_description = true;
            }

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

            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                m_has_pattern = true;
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                m_has_description = true;
            }

            return m_instances.access<Metadata>();
        }

        /**
        * Sets a description. Existing descriptions are overwritten
        * @tparam Metadata
        * @return
        */
        template <class Metadata> bool has() {
            return m_instances.has<Metadata>();
        }

        bool has_pattern() const {
            return m_has_pattern;
        }

        bool has_description() const {
            return m_has_description;
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
        mutable cxxh::drilldown_singleton_map<misa_serializeable> m_instances;

        bool m_has_pattern = false;

        bool m_has_description = false;

    public:

       /**
        * Convenience function that creates a description storage initialized with the provided values
        * @tparam Arg
        * @tparam Args
        * @param arg
        * @param args
        * @return
        */
        template<typename Arg, typename... Args> static std::shared_ptr<misa_description_storage> with(Arg &&arg, Args&&... args) {
            using arg_type = typename std::remove_reference<Arg>::type;
            static_assert(std::is_base_of<misa_data_description, arg_type>::value || std::is_base_of<misa_data_pattern_base, arg_type>::value,
                          "Only patterns and descriptions can be attached!");
            if constexpr (sizeof...(Args) > 0) {
                auto storage = misa_description_storage::with(std::forward<Args>(args)...);
                storage->set(std::forward<Arg>(arg));
                return storage;
            }
            else {
                auto storage = std::make_shared<misa_description_storage>();
                storage->set(std::forward<Arg>(arg));
                return storage;
            }
        }

    };

    inline void to_json(nlohmann::json& j, const misa_description_storage& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_description_storage& p) {
        p.from_json(j);
    }
}
