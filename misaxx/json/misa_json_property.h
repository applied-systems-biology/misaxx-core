//
// Created by rgerst on 01.10.18.
//


#pragma once

#include "misa_json_property_base.h"
#include <optional>

namespace misaxx {
    /**
     * Additional metadata for JSON parameters
     * @tparam T
     */
    template<class T> struct misa_json_property : public misa_json_property_base {

        /**
         * If not empty, describes the allowed values.
         */
        std::vector<T> allowed_values;

        /**
         * Optional default value
         */
        std::optional<T> default_value;

        misa_json_property() = default;

        misa_json_property<T> &with_title(std::string t_title) {
            title = std::move(t_title);
            return *this;
        }

        misa_json_property<T> &with_description(std::string t_description) {
            description = std::move(t_description);
            return *this;
        }

        misa_json_property<T> &make_required() {
            required = true;
            return *this;
        }

        misa_json_property<T> &make_optional() {
            required = false;
            return *this;
        }

        misa_json_property<T> &make_enum(std::vector<T> t_allowed_values) {
            allowed_values = std::move(t_allowed_values);
            return *this;
        }

        misa_json_property<T> &with_default_value(T t_default_value) {
            default_value = std::move(t_default_value);
            return *this;
        }
    };
}