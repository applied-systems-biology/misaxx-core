//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/detail/misa_json_schema.h>

namespace misaxx {
    /**
     * An object that can be serialized and deserialized from/to JSON
     */
    struct misa_serializeable {
        virtual void from_json(const nlohmann::json &t_json) = 0;
        virtual void to_json(nlohmann::json &t_json) const = 0;
        virtual void to_json_schema(const misa_json_schema &t_schema) const = 0;
        virtual std::string get_serialization_id() const = 0;
    };
}
