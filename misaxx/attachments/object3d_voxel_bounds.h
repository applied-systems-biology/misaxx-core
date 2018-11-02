//
// Created by rgerst on 17.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/misa_serializeable.h>
#include "object3d_voxel_size.h"

namespace misaxx {
    struct object3d_voxel_bounds : public misa_serializeable {

        int min_x = std::numeric_limits<int>::max();
        int min_z = std::numeric_limits<int>::max();
        int min_y = std::numeric_limits<int>::max();
        int max_x = std::numeric_limits<int>::min();
        int max_z = std::numeric_limits<int>::min();
        int max_y = std::numeric_limits<int>::min();

        void to_json(nlohmann::json &t_json) const override {
            t_json["min-x"] = min_x;
            t_json["min-y"] = min_y;
            t_json["min-z"] = min_z;
            t_json["max-x"] = max_x;
            t_json["max-y"] = max_y;
            t_json["max-z"] = max_z;
        }

        void from_json(const nlohmann::json& j) override {
            min_x = j["min-x"];
            min_y = j["min-y"];
            min_z = j["min-z"];
            max_x = j["max-x"];
            max_y = j["max-y"];
            max_z = j["max-z"];
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

    void to_json(nlohmann::json& j, const object3d_voxel_bounds& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, object3d_voxel_bounds& p) {
        p.from_json(j);
    }
}