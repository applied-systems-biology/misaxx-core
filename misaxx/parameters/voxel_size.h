//
// Created by rgerst on 05.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <pattxx/metadata.h>
#include <misaxx/metadata/object3d_voxel_size.h>
#include "../object_node_path.h"

namespace misaxx {
    /**
     * Auto-parameter that contains the voxel size
     */
    struct voxel_size : public metadata::object3d_voxel_size {
        using configuration_namespace_type = object_node_path;
        static inline const std::string name = "voxel-size";
        static inline const pattxx::metadata metadata = pattxx::metadata("Voxel size");
    };

    void to_json(nlohmann::json& j, const voxel_size& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, voxel_size& p) {
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