//
// Created by rgerst on 01.10.18.
//


#pragma once

#include <vector>
#include <string>

namespace misaxx {

    /**
     * Describes the content of a JSON parameter
     */
    struct misa_json_property_base {

        /**
         * A short descriptive title
         */
        std::string title;

        /**
         * A short explanation what the property does
         */
        std::string description;

        /**
         * If true, the property must be set by the user
         */
        bool required = false;

        misa_json_property_base() = default;
    };
}
