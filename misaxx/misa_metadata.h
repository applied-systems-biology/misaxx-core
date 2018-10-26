//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx {
    /**
     * Metadata that can be dynamically serialized from/to JSON
     */
    struct misa_metadata {
        virtual void from_json(const nlohmann::json &t_json) = 0;
        virtual nlohmann::json to_json() const = 0;
        virtual std::string get_name() const = 0;
    };
}
