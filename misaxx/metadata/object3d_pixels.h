//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_metadata.h"

namespace misaxx {
    /**
     * Attachable metadata containing the number of pixels of a 3D object
     */
    struct object3d_pixels : public misa_metadata {
        int pixels = 0;

        nlohmann::json to_json() const override {
            nlohmann::json j;
            j["pixels"] = pixels;
            return j;
        }

        std::string get_name() const override {
            return "object3d-pixels";
        }
    };

    void to_json(nlohmann::json& j, const object_name& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, object_name& p) {
        p.name = j;
    }
}
