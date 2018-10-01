//
// Created by rgerst on 17.09.18.
//


#pragma once

#include <stdexcept>
#include <nlohmann/json.hpp>
#include "misa_metadata.h"
#include "../object_node_path.h"

namespace misaxx {
    struct object3d_voxel_size : public misa_metadata {

        using configuration_namespace_type = object_node_path;

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

    void to_json(nlohmann::json& j, const object3d_voxel_size& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, object3d_voxel_size& p) {
        nlohmann::json::const_iterator xz = j.find("xz");
        nlohmann::json::const_iterator x = j.find("x");
        nlohmann::json::const_iterator y = j.find("y");
        nlohmann::json::const_iterator z = j.find("z");
        if(xz != j.end()) {
            p.x = p.z = xz.value();
        }
        if(x != j.end()) {
            p.x = x.value();
        }
        if(y != j.end()) {
            p.y = y.value();
        }
        if(z != j.end()) {
            p.z = z.value();
        }
    }
}