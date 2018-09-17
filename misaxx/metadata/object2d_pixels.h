//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misa_metadata.h"

namespace misaxx::metadata {
    /**
     * Attachable metadata containing the number of pixels of a 2D object
     */
    struct object2d_pixels : public misa_metadata {
        int pixels = 0;

        nlohmann::json to_json() const override {
            nlohmann::json j;
            j["pixels"] = pixels;
            return j;
        }

        std::string get_name() const override {
            return "object2d-pixels";
        }
    };
}
