//
// Created by rgerst on 17.09.18.
//


#pragma once

#include <stdexcept>
#include <nlohmann/json.hpp>
#include "misa_metadata.h"

namespace misaxx::metadata {
    struct object3d_voxel_size : public misa_metadata {
        double x = 0;
        double y = 0;
        double z = 0;

        double xz() const {
            if(x != z)
                throw std::runtime_error("x and z must be the same!");
            return x;
        }

        double volume() const {
            return x * y * z;
        }

        nlohmann::json to_json() const override {
            nlohmann::json j;
            j["x"] = x;
            j["y"] = y;
            j["z"] = z;
            return j;
        }

        std::string get_name() const override {
            return "object3d-voxel-size";
        }
    };
}