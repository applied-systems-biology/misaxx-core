//
// Created by rgerst on 16.01.19.
//


#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <misaxx/json/misa_json_property.h>
#include <misaxx/runtime/misa_runtime_properties.h>
#include <misaxx/json/misa_json_schema_builder.h>
#include <misaxx/misa_serializeable.h>
#include <misaxx/misa_primitive.h>

namespace misaxx::parameter_registry {
    extern nlohmann::json &get_parameter_json();

    extern nlohmann::json get_json_raw(const std::vector<std::string> &t_path);

    extern misa_json_schema_builder &get_schema_builder();

    /**
      * Gets the converted JSON value of given path. If the value is not defined in JSON, an exception is thrown.
      * @tparam T
      * @tparam Converter Converter that should be used
      * @param t_path
      * @return
      */
    template<typename T> inline T get_json(const std::vector<std::string> &t_path, const misa_json_property<T> &t_json_metadata) {
        const bool is_simulating = misaxx::runtime_properties::is_simulating();

        if(is_simulating) {
            get_schema_builder().insert<T>(t_path, t_json_metadata);
            if constexpr (std::is_base_of<misa_serializeable, T>::value) {
                T().to_json_schema(misa_json_schema(get_schema_builder(), t_path));
            }
            else {
                misa_primitive<T> v;
                v.metadata = t_json_metadata;
                v.to_json_schema(misa_json_schema(get_schema_builder(), t_path));
            }
        }

        auto json = get_json_raw(t_path);
        if(json.empty()) {
            if(t_json_metadata.default_value) {
                return t_json_metadata.default_value.value();
            }
            else if(is_simulating) {
                return T();
            }
            else {
                throw std::runtime_error(boost::algorithm::join(t_path, " / ") + " does not exist!");
            }
        }
        else {
            return json.get<T>();
        }
    }

    /**
     * Registers a parameter into the JSON schema
     * @tparam T
     * @param t_path
     * @param t_json_metadata
     */
    template<typename T> inline void register_parameter(const std::vector<std::string> &t_path, const misa_json_property<T> &t_json_metadata) {
        const bool is_simulating = misaxx::runtime_properties::is_simulating();
        if(is_simulating) {
            get_schema_builder().insert<T>(t_path, t_json_metadata);
            if constexpr (std::is_base_of<misa_serializeable, T>::value) {
                T().to_json_schema(misa_json_schema(get_schema_builder(), t_path));
            }
            else {
                misa_primitive<T> v;
                v.metadata = t_json_metadata;
                v.to_json_schema(misa_json_schema(get_schema_builder(), t_path));
            }
        }
    }
}