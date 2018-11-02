//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx {
    /**
     * An object that can be serialized and deserialized from/to JSON
     */
    struct misa_serializeable {
        virtual void from_json(const nlohmann::json &t_json) = 0;
        virtual void to_json(nlohmann::json &t_json) const = 0;
        virtual std::string get_name() const = 0;
    };
}
