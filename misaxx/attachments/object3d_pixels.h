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

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "attachments/object3d/pixels");
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("pixels").declare_required<int>();
        }
    };

    void to_json(nlohmann::json& j, const object3d_pixels& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, object3d_pixels& p) {
        p.from_json(j);
    }
}
