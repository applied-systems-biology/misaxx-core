//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "object3d_voxel_size.h"
#include "object3d_pixels.h"

namespace misaxx {
    struct object3d_volume : public misa_metadata {
        double volume = 0;

        object3d_volume() = default;
        explicit object3d_volume(const object3d_pixels &t_pixels, const object3d_voxel_size &t_voxel_size) : volume(t_pixels.pixels * t_voxel_size.volume()) {

        }

        nlohmann::json to_json() const override {
            nlohmann::json j;
            j["volume"] = volume;
            return j;
        }

        void from_json(const nlohmann::json& j) override {
            volume = j["volume"];
        }

        std::string get_name() const override {
            return "object3d-volume";
        }
    };

    void to_json(nlohmann::json& j, const object3d_volume& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, object3d_volume& p) {
        p.from_json(j);
    }
}
