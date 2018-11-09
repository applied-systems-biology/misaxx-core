//
// Created by rgerst on 03.08.18.
//


#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "misa_json_property.h"

/**
 * Contains functions that write a parameter into a JSON schema schema
 */
namespace misaxx {

    using path_t = std::vector<std::string>;

    /**
     * Workaround until we get proper json_schema type TODO
     * @param t_input
     * @return
     */
    inline nlohmann::json json_as_property(const nlohmann::json &t_input) {
        nlohmann::json result;
        result["type"] = t_input.type_name();

        if(t_input.is_object()) {
            for(auto it = t_input.begin(); it != t_input.end(); ++it) {
                result["properties"][it.key()] = json_as_property(it.value());
            }
        }
        return result;
    }

    template<typename T> struct schema_converter {

        static nlohmann::json as_property(const misa_json_property<T>& t_metadata) {
            nlohmann::json result = nlohmann::json({});
            auto type_as_json = nlohmann::json(T());

            result["type"] = type_as_json.type_name();

            if(!t_metadata.title.empty()) {
                result["title"] = t_metadata.title;
            }
            if(!t_metadata.description.empty()) {
                result["description"] = t_metadata.description;
            }

            // If we have enumerated values, use them
            if(!t_metadata.allowed_values.empty()) {
                result["enum"] = t_metadata.allowed_values;
            }

            // TODO: Workaround for metadata
            if(type_as_json.is_object()) {
                for(auto it = type_as_json.begin(); it != type_as_json.end(); ++it) {
                    result["properties"][it.key()] = json_as_property(it.value());
                 }
            }

            return result;
        }

    };

}