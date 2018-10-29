//
// Created by rgerst on 26.10.18.
//


#pragma once

#include "../../misa_metadata.h"
#include "misa_file_description.h"

namespace misaxx {
    struct misa_file_stack_description : public misa_metadata {

        misa_file_description file_template;
        std::unordered_map<std::string, misa_file_description> files;

        misa_file_stack_description() = default;

        explicit misa_file_stack_description(misa_file_description t_file_template) : file_template(std::move(t_file_template)) {

        }

        explicit misa_file_stack_description(std::unordered_map<std::string, misa_file_description> t_files) : files(std::move(t_files)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            for(auto it = t_json.find("filetype"); it != t_json.end(); ++it) {
                files[it.key()] = it.value();
            }
            file_template.from_json(t_json);
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["files"] = files;
            file_template.to_json(t_json);
        }

        std::string get_name() const override {
            return "misa-file-stack";
        }

        bool has_files() const {
            return !files.empty();
        }
    };

    void to_json(nlohmann::json& j, const misa_file_stack_description& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_file_stack_description& p) {
        p.from_json(j);
    }
}
