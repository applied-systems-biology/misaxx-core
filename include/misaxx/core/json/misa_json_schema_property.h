//
// Created by ruman on 25.02.19.
//

#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/core/misa_serialization_id.h>
#include <misaxx/core/misa_serializable.h>

namespace misaxx {

    struct misa_json_schema_property {

        /**
         * Subtree-properties are supposed to have no default value and no child template.
         * Leaf properties can have a child template and default value, but no child properties.
         */
        enum class property_type {
            subtree,
            leaf
        };

        /**
         * Type of this JSON schema property
         */
        property_type property_type = property_type::subtree;

        /**
         * Type of the JSON entry descibed by this schema
         */
        nlohmann::json::value_t value_type = nlohmann::json::value_t::object;

        /**
        * A short descriptive title
        */
        std::string title = "";

        /**
         * A short explanation what the schema/object does
         */
        std::string description = "";

        /**
         * True if the value of this schema object must be set by the user
         */
        bool required = false;

        /**
         * Optional default value of this property
         */
        std::optional<nlohmann::json> default_value = std::nullopt;

        /**
         * Optional description of child entries if this schema describes an object or an array
         */
        std::shared_ptr<misa_json_schema_property> children_template;

        /**
         * Properties if this schema describes an object
         * Only valid if the property_type is of type "object"
         */
        std::unordered_map<std::string, std::shared_ptr<misa_json_schema_property>> properties;

        /**
         * Information about the current object if its is a misaxx::misa_serializable
         */
        std::optional<misa_serialization_id> serialization_id;

        /**
         * Information about the current object if its is a misaxx::misa_serializable
         */
        std::optional<std::vector<misa_serialization_id>> serialization_hierarchy;

        /**
         * Additional properties that are merged into the schema
         */
        nlohmann::json annotation;

        /**
         * Converts this JSON schema property to a proper JSON JSON
         * @return
         */
        nlohmann::json to_json() const;

        /**
         * Declares this JSON schema property as the given type
         * Supports misaxx::misa_serializable
         * @tparam T
         * @return
         */
        template<typename T>
        misa_json_schema_property &declare() {
            if constexpr (std::is_base_of<misa_serializable, T>::value) {
                property_type = property_type::subtree;
                value_type = nlohmann::json::value_t::object;
                dynamic_cast<const misa_serializable&>(T()).to_json_schema(*this);
            }
            else if constexpr (nlohmann::detail::is_compatible_array_type<nlohmann::json, T>::value) {
                property_type = property_type::leaf;
                value_type = nlohmann::json::value_t::array;
                // Extract the "items" property from the type as children_template
                children_template = std::make_shared<misa_json_schema_property>();
                children_template->declare<typename T::value_type>();
            }
            else if constexpr (nlohmann::detail::is_compatible_object_type<nlohmann::json, T>::value) {
                property_type = property_type ::leaf;
                value_type = nlohmann::json::value_t::object;
                // Extract the "additionalProperties" property from the type as children_template
                children_template = std::make_shared<misa_json_schema_property>();
                children_template->declare<typename T::mapped_type>();
            }
            else {
                property_type = property_type::leaf;
                value_type = nlohmann::json(T()).type();
            }
            return *this;
        }

        /**
         * Creates and returns a sub-property with given key if this schema is a subtree-object
         * @param key
         * @return
         */
        misa_json_schema_property &resolve(const std::string &key);

        /**
         * Shortcut for .resolve()
         * @param key
         * @return
         */
        misa_json_schema_property &operator [](const std::string &key) {
            return resolve(key);
        }

        /**
         * Shortcut for declare<T>() with T being the argument
         * @tparam T
         * @param value
         * @return
         */
        template<typename T>
        misa_json_schema_property &operator=(const T &) {
           return declare<T>();
        }

    };
}


