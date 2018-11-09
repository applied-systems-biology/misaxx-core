//
// Created by rgerst on 03.08.18.
//


#pragma once

#include <boost/algorithm/string/join.hpp>
#include "misa_json_schema_converter.h"
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace misaxx {

    /**
     * Encapsulates writing a JSON schema
     */
    struct misa_json_schema_builder {

        using path_t = std::vector<std::string>;

        static nlohmann::json::json_pointer to_json_pointer(const path_t &t_path, const std::string &after = "") {
            return nlohmann::json::json_pointer("/" + boost::algorithm::join(t_path, "/") + after);
        }

        /**
         * Returns the path to the JSON schema definition property of given parameter
         * @param t_parameter_path
         * @return
         */
        path_t schema_property_path(const path_t &t_parameter_path) {
            path_t base_path;
            for(size_t i = 0; i < t_parameter_path.size(); ++i) {
                base_path.emplace_back("properties");
                base_path.push_back(t_parameter_path[i]);
            }           
            return base_path;
        }

        /**
         * Returns the path to the JSON schema definition property of given parameter
         * @param t_parameter_path
         * @return
         */
        path_t schema_parent_path(const path_t &t_parameter_path) {
            path_t base_path;
            for(size_t i = 0; i < t_parameter_path.size() - 1; ++i) {
                base_path.emplace_back("properties");
                base_path.push_back(t_parameter_path[i]);
            }
            return base_path;
        }

        /**
         * Ensures that the necessary property paths for a parameter exist and "type" : "object" is set
         * @param t_parameter_path
         */
        void ensure_schema_property_path(const path_t &t_parameter_path) {
            path_t base_path;
            for(size_t i = 0; i < t_parameter_path.size() - 1; ++i) {
                base_path.emplace_back("properties");
                base_path.push_back(t_parameter_path[i]);

                // Set the pointer accordingly
                data[to_json_pointer(base_path, "/type")] = "object";
            }
        }


        /**
         * Inserts a node into the schema
         * @param t_path
         * @param t_metadata
         */
        void insert_node(const path_t& t_path, const misa_json_property_base &t_metadata) {
            path_t base_path;
            for(size_t i = 0; i < t_path.size(); ++i) {
                base_path.emplace_back("properties");
                base_path.push_back(t_path[i]);

                // Set the pointer accordingly
                data[to_json_pointer(base_path, "/type")] = "object";
            }

            if(!t_metadata.title.empty()) {
                data[to_json_pointer(base_path, "/title")] = t_metadata.title;
            }
            if(!t_metadata.description.empty()) {
                data[to_json_pointer(base_path, "/description")] = t_metadata.description;
            }
        }

        /**
         * Inserts a schema definition for given path
         * @tparam T
         * @param t_path
         * @param t_metadata
         */
        template<typename T> void insert(const path_t &t_parameter_path, const misa_json_property<T> &t_json_metadata) {
            ensure_schema_property_path(t_parameter_path);
            auto as_json = nlohmann::json(T());

            const auto base_path = schema_property_path(t_parameter_path);
            data[to_json_pointer(base_path)] = schema_converter<T>::as_property(t_json_metadata);
        }

        /**
         * Inserts schema definition for given path and sets it as required
         * @tparam T
         * @param t_path
         * @param t_metadata
         */
        template<typename T> void insert_required(const path_t &t_parameter_path, const misa_json_property<T> &t_json_metadata) {
            insert<T>(t_parameter_path, t_json_metadata);
            const auto base_path = schema_parent_path(t_parameter_path);
            const auto ptr = to_json_pointer(base_path, "/required");

            if(data[ptr].empty()) {
                data[ptr] = { t_parameter_path[t_parameter_path.size() - 1] };
            }
            else {
                const auto &item = t_parameter_path[t_parameter_path.size() - 1];
                for(const auto &v : data[ptr]) {
                    if(v == item)
                        return;
                }
                data[ptr].push_back(item);
            }
        }

        /**
         * Inserts schema definition and sets the default value
         * @tparam T
         * @param t_path
         * @param t_default
         * @param t_metadata
         */
        template<typename T> void insert_optional(const path_t &t_parameter_path, const T& t_default, const misa_json_property<T> &t_json_metadata) {
            insert<T>(t_parameter_path, t_json_metadata);
            const auto base_path = schema_property_path(t_parameter_path);
            data[to_json_pointer(base_path, "/default")] = nlohmann::json(t_default);
        }

        /**
         * Inserts a static value (required with default value)
         * @tparam T
         * @param t_parameter_path
         * @param t_value
         * @param t_json_metadata
         */
        template<typename T> void insert_static(const path_t &t_parameter_path, const T& t_value, const misa_json_property<T> &t_json_metadata) {
            insert<T>(t_parameter_path, t_json_metadata);
            const auto base_path = schema_parent_path(t_parameter_path);
            const auto ptr = to_json_pointer(base_path, "/required");

            const auto base_path2 = schema_property_path(t_parameter_path);
            data[to_json_pointer(base_path2, "/default")] = nlohmann::json(t_value);

            if(data[ptr].empty()) {
                data[ptr] = { t_parameter_path[t_parameter_path.size() - 1] };
            }
            else {
                data[ptr].push_back(t_parameter_path[t_parameter_path.size() - 1]);
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
        void write(const boost::filesystem::path &t_path) {
            std::ofstream out;
            out.open(t_path.string());
            out << std::setw(4) << data;
            out.close();
        }

    };

}