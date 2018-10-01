//
// Created by rgerst on 01.10.18.
//


#pragma once

#include <string>
#include "misa_metadata.h"
#include "../object_node_path.h"

namespace misaxx {

    struct object_name : public misa_metadata {

        using configuration_namespace_type = object_node_path;

        std::string name;

        object_name() = default;

        explicit object_name(std::string t_name) : name(std::move(t_name)) {
        }

        nlohmann::json to_json() const override {
            return name;
        }

        std::string get_name() const override {
            return "object-name";
        }
    };

    void to_json(nlohmann::json& j, const object_name& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, object_name& p) {
        p.name = j;
    }
}