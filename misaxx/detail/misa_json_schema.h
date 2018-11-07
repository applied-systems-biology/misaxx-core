//
// Created by rgerst on 07.11.18.
//


#pragma once

#include <pattxx/json/json_schema_builder.h>

namespace misaxx {
    /**
     * Helper class around pattxx JSON schema for serialization.
     * This will be later replaced by a proper json_schema.
     */
    class misa_json_schema {
    public:
        misa_json_schema(pattxx::json::json_schema_builder &t_builder, pattxx::json::path_t t_path) :
        m_builder(&t_builder), m_path(std::move(t_path)) {

        }

        pattxx::json::json_schema_builder &get_builder() const {
            return *m_builder;
        }

        template<class... Args> misa_json_schema resolve(const Args&... t_segment) const {
            pattxx::json::path_t new_path = m_path;
            for(const auto &segment :  { t_segment... }) {
                new_path.push_back(segment);
            }
            return misa_json_schema(get_builder(), std::move(new_path));
        }

        template<typename T> void declare_optional(const std::string &t_name, const T &t_default = T(), const pattxx::json::json_property<T> &t_json_metadata = pattxx::json::json_property<T>()) const  {
            m_builder->insert_optional<T>(resolve(t_name).m_path, t_default, t_json_metadata);
        }

        template<typename T> void declare_required(const std::string &t_name, const pattxx::json::json_property<T> &t_json_metadata = pattxx::json::json_property<T>()) const {
            m_builder->insert_required<T>(resolve(t_name).m_path, t_json_metadata);
        }

        template<typename T> void define(const std::string &t_name, const T &t_value, const pattxx::json::json_property<T> &t_json_metadata = pattxx::json::json_property<T>()) const {
            m_builder->insert_static<T>(resolve(t_name).m_path, t_value, t_json_metadata);
        }

    private:
        pattxx::json::json_schema_builder *m_builder;
        pattxx::json::path_t m_path;
    };
}
