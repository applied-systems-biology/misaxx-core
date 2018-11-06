//
// Created by rgerst on 01.10.18.
//


#pragma once

#include <string>
#include <misaxx/misa_serializeable.h>
#include "misaxx/parameters/object_node_path.h"

namespace misaxx {

    struct object_name : public misa_serializeable {

        using configuration_namespace_type = object_node_path;

        std::string name;

        object_name() = default;

        explicit object_name(std::string t_name) : name(std::move(t_name)) {
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json = name;
        }

        void from_json(const nlohmann::json& j) override {
            name = j;
        }

        std::string get_name() const override {
            return "object-name";
        }
    };

    void to_json(nlohmann::json& j, const object_name& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, object_name& p) {
        p.from_json(j);
    }
}