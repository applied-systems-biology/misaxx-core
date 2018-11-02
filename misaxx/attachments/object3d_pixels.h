//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * Attachable metadata containing the number of pixels of a 3D object
     */
    struct object3d_pixels : public misa_serializeable {
        int pixels = 0;

        void to_json(nlohmann::json &t_json) const override {
            t_json["pixels"] = pixels;
        }

        void from_json(const nlohmann::json& j) override {
            pixels = j["pixels"];
        }

        std::string get_name() const override {
            return "object3d-pixels";
        }
    };

    void to_json(nlohmann::json& j, const object3d_pixels& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, object3d_pixels& p) {
        p.from_json(j);
    }
}
