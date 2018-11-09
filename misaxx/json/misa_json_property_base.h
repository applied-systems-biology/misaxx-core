//
// Created by rgerst on 01.10.18.
//


#pragma once

#include <vector>
#include <string>

namespace misaxx {

    struct misa_json_property_base {

        std::string title;

        std::string description;

        misa_json_property_base() = default;

        explicit misa_json_property_base(std::string t_title, std::string t_description = "") : title(std::move(t_title)), description(std::move(t_description)) {
        }
    };
}
