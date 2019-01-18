//
// Created by rgerst on 25.10.18.
//


#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <misaxx/misa_data_pattern_base.h>
#include <misaxx/misa_data_description.h>
#include <misaxx/misa_serializeable.h>
#include <misaxx/utils/type_traits.h>

namespace misaxx {
    /**
     * Metadata that is attached to filesystem entries.
     * For flexibility, the filesystem metadata holds the raw JSON data that can be interpreted as misa_metadata instances
     */
    struct misa_description_storage : public misa_serializeable {

        misa_description_storage() = default;

        misa_description_storage(const misa_description_storage &t_source);

        misa_description_storage(misa_description_storage && t_source) = default;

        misa_description_storage &operator =(const misa_description_storage &t_source);

        misa_description_storage &operator =(misa_description_storage &&t_source) = default;

        /**
         * Clones this metadata
         * @return
         */
        std::shared_ptr<misa_description_storage> clone() const;

        /**
         * Loads from an imported JSON where all the contents in the JSON can be pattern information.
         * If a pattern or description are existing, the standard deserialization is used.
         * @param t_json
         */
        void from_imported_json(const nlohmann::json &t_json);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    public:

        /**
         * Gets a description from this metadata instance. It is deserialized from the
         * internal JSON if needed.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> const Metadata &get() const {
            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_pattern))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                } else if (misa_serializeable::type_is_deserializeable_from_json<Metadata>(m_raw_pattern_json)) {
                    Metadata m;
                    m.from_json(m_raw_pattern_json);
                    m_pattern = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                }
                else {
                    throw std::runtime_error(std::string("The description storage does not contain ") + typeid(Metadata).name());
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_description))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                } else if (misa_serializeable::type_is_deserializeable_from_json<Metadata>(m_raw_description_json)) {
                    Metadata m;
                    m.from_json(m_raw_description_json);
                    m_description = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                }
                else {
                    throw std::runtime_error(std::string("The description storage does not contain ") + typeid(Metadata).name());
                }
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
        * Gets a description from this metadata instance. It is deserialized from the
        * internal JSON if needed.
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &get() {
            if(has<Metadata>()) {
                return access<Metadata>();
            }
            else {
                throw std::runtime_error(std::string("The description storage does not contain ") + typeid(Metadata).name());
            };
        }

        /**
         * Sets a description. Existing descriptions are overwritten
         * @tparam Metadata
         * @return
         */
        template <class Metadata> Metadata &set(Metadata t_description) {
            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                m_pattern = std::make_shared<Metadata>(std::move(t_description));
                return *std::dynamic_pointer_cast<Metadata>(m_pattern);
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                m_description = std::make_shared<Metadata>(std::move(t_description));
                return *std::dynamic_pointer_cast<Metadata>(m_description);
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
        * Returns the pattern or description. If necessary, create it.
        * If there is already an existing pattern or description with a differnt type hierarchy, the stored value is overwritten
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &access() {

            std::vector<misa_serialization_id> serialization_hierarchy = Metadata().get_serialization_id_hierarchy();

            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_pattern))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                } else {
                    Metadata m;
                    if (misa_serializeable::type_is_deserializeable_from_json<Metadata>(m_raw_pattern_json)) {
                        m.from_json(m_raw_pattern_json);
                    }
                    m_pattern = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_description))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                } else {
                    Metadata m;
                    if (misa_serializeable::type_is_deserializeable_from_json<Metadata>(m_raw_description_json)) {
                        m.from_json(m_raw_description_json);
                    }
                    m_description = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                }
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
        * Returns true if the storage or the underlying JSON data contains a specific pattern or description
        * @tparam Metadata
        * @return
        */
        template <class Metadata> bool has() const {
            if constexpr (std::is_base_of<misa_data_pattern_base, Metadata>::value) {
                if(static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_pattern))) {
                    return true;
                }
                else {
                    return misa_serializeable::type_is_deserializeable_from_json<Metadata>(m_raw_pattern_json);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if(static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_description))) {
                    return true;
                }
                else {
                    return misa_serializeable::type_is_deserializeable_from_json<Metadata>(m_raw_description_json);
                }
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
         * Returns true if the description storage has a pattern
         * @return
         */
        bool has_pattern() const;

        /**
         * Returns true if the description storage has a description
         * @return
         */
        bool has_description() const;

    private:

        /**
         * Raw JSON carried along from the settings to allow flexible importing (upgrading) of information during processing
         */
        nlohmann::json m_raw_pattern_json;

        /**
        * Raw JSON carried along from the settings to allow flexible importing (upgrading) of information during processing
        */
        nlohmann::json m_raw_description_json;

        mutable std::shared_ptr<misa_data_pattern_base> m_pattern;

        mutable std::shared_ptr<misa_data_description> m_description;

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
