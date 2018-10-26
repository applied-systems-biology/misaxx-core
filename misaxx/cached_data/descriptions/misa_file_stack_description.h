//
// Created by rgerst on 26.10.18.
//


#pragma once

#include "../../misa_metadata.h"
#include "misa_file_description.h"

namespace misaxx {
    struct misa_file_stack_description : public misa_metadata {

        std::unordered_map<std::string, misa_file_description> files;

        void from_json(const nlohmann::json &t_json) override {
            files = t_json;
        }

        nlohmann::json to_json() const override {
            return files;
        }

        std::string get_name() const override {
            return "misa-file-stack";
        }
    };

    void to_json(nlohmann::json& j, const misa_file_stack_description& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, misa_file_stack_description& p) {
        p.from_json(j);
    }
}
