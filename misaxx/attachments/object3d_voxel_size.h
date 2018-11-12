//
// Created by rgerst on 17.09.18.
//


#pragma once

#include <stdexcept>
#include <nlohmann/json.hpp>
#include <misaxx/misa_serializeable.h>
#include <misaxx/workers/paths/object_node_path.h>

namespace misaxx {
    struct object3d_voxel_size : public misa_serializeable {

        using configuration_namespace_type = object_node_path;

        double x = 0;
        double y = 0;
        double z = 0;

        double xz() const {
            if(x != z)
                throw std::runtime_error("x and z must be the same!");
            return x;
        }

        double volume() const {
            return x * y * z;
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["x"] = x;
            t_json["y"] = y;
            t_json["z"] = z;
        }

        void from_json(const nlohmann::json& j) override {
            nlohmann::json::const_iterator xz = j.find("xz");
            nlohmann::json::const_iterator x = j.find("x");
            nlohmann::json::const_iterator y = j.find("y");
            nlohmann::json::const_iterator z = j.find("z");
            if(xz != j.end()) {
                this->x = this->z = xz.value();
            }
            if(x != j.end()) {
                this->x = x.value();
            }
            if(y != j.end()) {
                this->y = y.value();
            }
            if(z != j.end()) {
                this->z = z.value();
            }
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "attachments/object3d/voxel-size");
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("x").declare_required<double>();
            t_schema.resolve("y").declare_required<double>();
            t_schema.resolve("z").declare_required<double>();
        }
    };

    void to_json(nlohmann::json& j, const object3d_voxel_size& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, object3d_voxel_size& p) {
        p.from_json(j);
    }
}