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

        std::string title;

        std::string description;

        bool required = false;

        misa_json_property_base() = default;
    };
}
