//
// Created by rgerst on 07.11.18.
//


#pragma once

#include <misaxx/json/misa_json_schema_builder.h>
#include <misaxx/json/misa_json_property.h>

namespace misaxx {
    /**
     * Helper class around pattxx JSON schema for serialization.
     * This will be later replaced by a proper json_schema.
     */
    class misa_json_schema {
    public:
        misa_json_schema(misa_json_schema_builder &t_builder, std::vector<std::string> t_path) :
                m_builder(&t_builder), m_path(std::move(t_path)) {

        }

        misa_json_schema_builder &get_builder() const {
            return *m_builder;
        }

        template<class... Args> misa_json_schema resolve(const Args&... t_segment) const {
            std::vector<std::string> new_path = m_path;
            for(const std::string &segment : std::initializer_list<std::string>({ t_segment... })) {
                new_path.push_back(segment);
            }
            return misa_json_schema(get_builder(), std::move(new_path));
        }

        misa_json_schema parent() const {
            if(m_path.empty())
                throw std::runtime_error("JSON schema is already parent!");
            std::vector<std::string> new_path = m_path;
            if(new_path.size() > 1) {
                std::swap(new_path[0], new_path[new_path.size() - 1]);
                new_path.erase(new_path.end() - 1);
            } else {
                new_path.clear();
            }
            return misa_json_schema(get_builder(), std::move(new_path));
        }

        /**
         * Declares this path as optional value with default
         * @tparam T
         * @param t_default
         * @param t_json_metadata
         */
        template<typename T> void declare(misa_json_property<T> t_json_metadata = misa_json_property<T>()) const  {
            m_builder->insert<T>(m_path, std::move(t_json_metadata));
        }

        /**
         * Declares this path as optional value with default
         * @tparam T
         * @param t_default
         * @param t_json_metadata
         */
        template<typename T> void declare_optional(const T &t_default = T(), misa_json_property<T> t_json_metadata = misa_json_property<T>()) const  {
            t_json_metadata.default_value = t_default;
            t_json_metadata.required = false;
            declare<T>(std::move(t_json_metadata));
        }

        /**
         * Declares this path as required value
         * @tparam T
         * @param t_json_metadata
         */
        template<typename T> void declare_required(misa_json_property<T> t_json_metadata = misa_json_property<T>()) const {
            t_json_metadata.required = true;
            declare<T>(std::move(t_json_metadata));
        }

        /**
         * Defines a static value for this path
         * @tparam T
         * @param t_value
         * @param t_json_metadata
         */
        template<typename T> void define(const T &t_value, misa_json_property<T> t_json_metadata = misa_json_property<T>()) const {
            t_json_metadata.default_value = t_value;
            t_json_metadata.required = true;
            declare<T>(std::move(t_json_metadata));
        }

    private:
        misa_json_schema_builder *m_builder;
        std::vector<std::string> m_path;
    };
}
