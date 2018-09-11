//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx {
    struct misa_metadata {
        virtual void to_json(nlohmann::json &target) const = 0;
        virtual std::string get_name() const = 0;

        void write_to_json_dict(nlohmann::json &target) const {
            to_json(target[get_name()]);
        }
    };
}
