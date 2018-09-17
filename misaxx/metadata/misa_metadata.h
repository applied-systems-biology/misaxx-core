//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx {
    struct misa_metadata {
        virtual nlohmann::json to_json() const = 0;
        virtual std::string get_name() const = 0;
    };
}
