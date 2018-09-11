//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx::metadata {
    /**
     * Attachable metadata containing the number of pixels of a 2D object
     */
    struct object2d_pixels {
        int pixels = 0;
    };

    void to_json(nlohmann::json &j, const object2d_pixels &o) {
        j["pixels"] = o.pixels;
    }

    void from_json(const nlohmann::json &j, object2d_pixels &o) {
        o.pixels = j["pixels"];
    }
}
