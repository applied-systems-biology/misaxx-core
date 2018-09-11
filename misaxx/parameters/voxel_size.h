//
// Created by rgerst on 05.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <pattxx/metadata.h>
#include "../object_node_path.h"

namespace misaxx {
    /**
     * Auto-parameter that contains the voxel size
     */
    struct voxel_size {

        using configuration_namespace_type = object_node_path;
        static inline const std::string name = "voxel-size";
        static inline const pattxx::metadata metadata = pattxx::metadata("Voxel size");

        double x;
        double y;
        double z;

        double xz() const {
            if(x != z)
                throw std::runtime_error("x and z must be the same!");
            return x;
        }

        double volume() const {
            return x * y * z;
        }
    };
}

namespace nlohmann {

    template<> struct adl_serializer<misaxx::voxel_size> {
        static void to_json(json& j, const misaxx::voxel_size& opt) {
            if(opt.x == opt.z) {
                j["xz"] = opt.x;
                j["y"] = opt.y;
            }
            else {
                j["x"] = opt.x;
                j["z"] = opt.z;
                j["y"] = opt.y;
            }
        }

        static void from_json(const json& j, misaxx::voxel_size& opt) {
            nlohmann::json::const_iterator xz = j.find("xz");
            nlohmann::json::const_iterator x = j.find("x");
            nlohmann::json::const_iterator y = j.find("y");
            nlohmann::json::const_iterator z = j.find("z");
            if(xz != j.end()) {
                opt.x = opt.z = xz.value();
            }
            if(x != j.end()) {
                opt.x = x.value();
            }
            if(y != j.end()) {
                opt.y = y.value();
            }
            if(z != j.end()) {
                opt.z = z.value();
            }
        }
    };

}