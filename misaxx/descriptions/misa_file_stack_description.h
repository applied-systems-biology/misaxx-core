//
// Created by rgerst on 26.10.18.
//


#pragma once

#include <misaxx/misa_serializeable.h>
#include <misaxx/descriptions/misa_file_description.h>
#include <misaxx/misa_data_description.h>

namespace misaxx {
    struct misa_file_stack_description : public misa_data_description {

        using files_type = std::unordered_map<std::string, misa_file_description>;

        files_type files;

        misa_file_stack_description() = default;

        explicit misa_file_stack_description(files_type t_files) : files(std::move(t_files)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            if(t_json.find("files") != t_json.end()) {
                for(auto it = t_json["files"].begin(); it != t_json["files"].end(); ++it) {
                    files[it.key()] = it.value();
                }
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["files"] = files;
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            for(const auto &kv : files) {
                kv.second.to_json_schema(t_schema.resolve("files", kv.first));
            }
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "descriptions/file-stack");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return create_serialization_id_hierarchy(*this, { misa_data_description::get_serialization_id_hierarchy() });
        }

    };

    void to_json(nlohmann::json& j, const misa_file_stack_description& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_file_stack_description& p) {
        p.from_json(j);
    }
}
