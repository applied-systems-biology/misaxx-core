//
// Created by rgerst on 17.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_metadata.h"
#include "object3d_voxel_size.h"

namespace misaxx::metadata {
    struct object3d_voxel_bounds : public misa_metadata {

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
            return "object3d-voxel-bounds";
        }

        bool is_valid() const {
            return max_x >= min_x && max_y >= min_y && max_z >= min_z;
        }

        object3d_voxel_size to_size() const {
            if(!is_valid())
                throw std::runtime_error("Invalid voxel bounds!");
            object3d_voxel_size v;
            v.x = max_x - min_x;
            v.y = max_y - min_y;
            v.z = max_z - min_z;
            return v;
        }
    };
}