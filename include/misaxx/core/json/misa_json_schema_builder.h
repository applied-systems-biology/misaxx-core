//
// Created by rgerst on 03.08.18.
//


#pragma once

#include <boost/algorithm/string/join.hpp>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <misaxx/core/json/misa_json_property.h>
#include <misaxx/core/json/misa_json_helper.h>
#include <iomanip>

namespace misaxx {

    struct misa_serializable;

    /**
     * Encapsulates writing a JSON schema
     */
    struct misa_json_schema_builder {

        using path_t = std::vector<std::string>;

        /**
         * Returns the path to the JSON schema definition property of given parameter
         * @param t_parameter_path
         * @return
         */
        path_t schema_property_path(const path_t &t_parameter_path);

        /**
         * Returns the path to the JSON schema definition property of given parameter
         * @param t_parameter_path
         * @return
         */
        path_t schema_parent_path(const path_t &t_parameter_path);

        /**
         * Ensures that the necessary property paths for a parameter exist and "type" : "object" is set
         * @param t_parameter_path
         */
        void ensure_schema_property_path(const path_t &t_parameter_path);

        /**
         * Annotates the parameter path with some information
         * This will add a a property into the raw JSON data and might not conform to the JSON schema standard
         * @tparam T
         * @param t_parameter_path
         * @param t_json_metadata
         */
        template<typename T> void annotate(const path_t &t_parameter_path, const std::string &t_annotation_name, const T &t_metadata) {
            ensure_schema_property_path(t_parameter_path);
            json_helper::access_json_path(data, schema_property_path(t_parameter_path))[t_annotation_name] = nlohmann::json(t_metadata);
        }

        /**
         * Inserts a schema definition for given path
         * @tparam T
         * @param t_path
         * @param t_metadata
         */
        template<typename T> void insert(const path_t &t_parameter_path, const misa_json_property<T> &t_json_metadata) {
            ensure_schema_property_path(t_parameter_path);

            nlohmann::json as_json;

            // If we have a serializable, prefer the dynamic method
            if constexpr (std::is_base_of<misa_serializable, T>::value) {
                T v;
                v.to_json(as_json);
            }
            else {
                as_json = nlohmann::json(T());
            }

            const auto property_base_path = schema_property_path(t_parameter_path);
            const auto property_parent_base_path = schema_parent_path(t_parameter_path);

            json_helper::access_json_path(data, property_base_path, "type") = as_json.type_name();

            // If the property is required, update the required list
            if(t_json_metadata.required) {
                nlohmann::json &required_list = json_helper::access_json_path(data, property_parent_base_path, "required");

                if(required_list.empty()) {
                    required_list = { t_parameter_path[t_parameter_path.size() - 1] };
                }
                else {
                    const auto &item = t_parameter_path[t_parameter_path.size() - 1];
                    for(const auto &v : required_list) {
                        if(v == item)
                            return;
                    }
                    required_list.push_back(item);
                }
            }

            // If we have enumerated values, use them
            if(!t_json_metadata.allowed_values.empty()) {
                json_helper::access_json_path(data, property_base_path, "enum") = t_json_metadata.allowed_values;
            }

            // If we have a default value, write it
            if(t_json_metadata.default_value) {
                json_helper::access_json_path(data, property_base_path, "default") = t_json_metadata.default_value.value();
            }
        }

        /**
         * Final JSON schema
         */
        nlohmann::json data = nlohmann::json({ {"type", "object"}, {"$schema", "http://json-schema.org/draft-07/schema#"} });

        /**
         * Saves the parameter schema to a file.
         * @param t_path
         */
        void write(const boost::filesystem::path &t_path);

    };

}