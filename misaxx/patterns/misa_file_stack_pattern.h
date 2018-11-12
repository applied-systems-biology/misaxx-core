//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/descriptions/misa_file_stack_description.h>
#include "misa_file_pattern.h"
#include <misaxx/misa_data_pattern.h>

namespace misaxx {
    struct misa_file_stack_pattern : public misa_data_pattern<misa_file_stack_description> {

        std::vector<boost::filesystem::path> extensions;

        misa_file_stack_pattern() = default;

        explicit misa_file_stack_pattern(std::vector<boost::filesystem::path> t_extensions) : extensions(std::move(t_extensions)) {

        }

        misa_file_stack_description produce(const boost::filesystem::path &t_directory) const {
            misa_file_stack_description result;
            for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_directory))) {
                for(const auto &extension : extensions) {
                    if(boost::iequals(entry.path().extension().string(), extension.string())) {
                        result.files.insert({ entry.path().filename().string(), misa_file_description(entry.path()) });
                        break;
                    }
                }
            }
            return result;
        }

        void from_json(const nlohmann::json &t_json) override {
            extensions.clear();
            for (const auto &i : t_json["extensions"]) {
                extensions.emplace_back(i.get<std::string>());
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            std::vector<std::string> extensions_;
            for(const auto &extension : extensions) {
                extensions_.emplace_back(extension.string());
            }
            t_json["extensions"] = extensions_;
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            std::vector<std::string> extensions_;
            for(const auto &extension : extensions) {
                extensions_.emplace_back(extension.string());
            }
            t_schema.resolve("extensions").declare_optional<std::vector<std::string>>(extensions_);
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "patterns/file-stack");
        }
    };

    void to_json(nlohmann::json& j, const misa_file_stack_pattern& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_file_stack_pattern& p) {
        p.from_json(j);
    }
}
