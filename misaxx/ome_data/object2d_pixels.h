//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include "misaxx/misa_metadata.h"

namespace misaxx {
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

        void from_json(const nlohmann::json &t_json) override {
            pixels = t_json["pixels"];
        }

        std::string get_name() const override {
            return "object2d-pixels";
        }
    };

    void to_json(nlohmann::json& j, const object2d_pixels& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, object2d_pixels& p) {
        p.from_json(j);
    }
}
