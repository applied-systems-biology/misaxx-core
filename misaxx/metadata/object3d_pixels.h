//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_metadata.h"

namespace misaxx::metadata {
    /**
     * Attachable metadata containing the number of pixels of a 3D object
     */
    struct object3d_pixels : public misa_metadata {
        int pixels = 0;

        void to_json(nlohmann::json &j) const override {
            j["pixels"] = pixels;
        }
    };
}
