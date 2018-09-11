//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/parameters/voxel_size.h>
#include "object3d_pixels.h"

namespace misaxx::metadata {
    struct object3d_volume : public misa_metadata {
        double volume = 0;

        object3d_volume() = default;
        explicit object3d_volume(const object3d_pixels &t_pixels, const voxel_size &t_voxel_size) : volume(t_pixels.pixels * t_voxel_size.volume()) {

        }

        void to_json(nlohmann::json &j) const override {
            j["volume"] = volume;
        }

        std::string get_name() const override {
            return "object3d-volume";
        }
    };
}
