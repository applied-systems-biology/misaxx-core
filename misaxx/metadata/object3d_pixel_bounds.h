//
// Created by rgerst on 17.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_metadata.h"

namespace misaxx::metadata {
    struct object3d_pixel_bounds : public misa_metadata {

        int min_x = 0;
        int min_z = 0;
        int min_y = 0;
        int max_x = 0;
        int max_z = 0;
        int max_y = 0;

        nlohmann::json to_json() const override {
            nlohmann::json j;
            j["min-x"] = min_x;
            j["min-y"] = min_y;
            j["min-z"] = min_z;
            j["max-x"] = max_x;
            j["max-y"] = max_y;
            j["max-z"] = max_z;
            return j;
        }

        std::string get_name() const override {
            return "object3d-pixel-bounds";
        }
    };
}