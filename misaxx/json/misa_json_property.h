//
// Created by rgerst on 01.10.18.
//


#pragma once

#include "misa_json_property_base.h"

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

        misa_json_property() = default;

        explicit misa_json_property(std::string t_title, std::string t_description = "") : misa_json_property_base(std::move(t_title), std::move(t_description)) {
        }
    };
}