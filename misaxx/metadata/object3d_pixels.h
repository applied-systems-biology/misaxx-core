//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx::metadata {
    /**
     * Attachable metadata containing the number of pixels of a 3D object
     */
    struct object3d_pixels {
        int pixels = 0;
    };

    void to_json(nlohmann::json &j, const object3d_pixels &o) {
        j["pixels"] = o.pixels;
    }

    void from_json(const nlohmann::json &j, object3d_pixels &o) {
        o.pixels = j["pixels"];
    }
}
