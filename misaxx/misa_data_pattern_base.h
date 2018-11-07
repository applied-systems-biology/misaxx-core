//
// Created by rgerst on 05.11.18.
//


#pragma once

#include "misa_serializeable.h"

namespace misaxx {
    struct misa_data_pattern_base : public misa_serializeable {
        void from_json(const nlohmann::json &t_json) override {
        }

        void to_json(nlohmann::json &t_json) const override {
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
        }

        std::string get_serialization_id() const override {
            return "misa-data-pattern-base";
        }
    };
}
