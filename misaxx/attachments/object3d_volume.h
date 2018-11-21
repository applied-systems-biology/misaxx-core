//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/attachments/object3d_voxel_size.h>
#include <misaxx/attachments/object3d_pixels.h>

namespace misaxx {
    struct object3d_volume : public misa_serializeable {
        double volume = 0;

        object3d_volume() = default;
        explicit object3d_volume(const object3d_pixels &t_pixels, const object3d_voxel_size &t_voxel_size) : volume(t_pixels.pixels * t_voxel_size.volume()) {

        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["volume"] = volume;
        }

        void from_json(const nlohmann::json& j) override {
            volume = j["volume"];
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "attachments/object3d/volume");
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("volume").declare_required<double>();
        }
    };

    void to_json(nlohmann::json& j, const object3d_volume& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, object3d_volume& p) {
        p.from_json(j);
    }
}
