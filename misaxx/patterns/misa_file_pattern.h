//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/misa_data_pattern.h>
#include <misaxx/descriptions/misa_file_description.h>

namespace misaxx {

    /**
     * A file pattern is a description that allows
     */
    struct misa_file_pattern : public misa_data_pattern<misa_file_description> {

        boost::filesystem::path filename;
        std::vector<boost::filesystem::path> extensions;

        misa_file_pattern() = default;

        explicit misa_file_pattern(std::vector<boost::filesystem::path> t_extensions) :
        extensions(std::move(t_extensions)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            if(t_json.find("filename") != t_json.end())
                filename = t_json["filename"].get<std::string>();
            if(t_json.find("extensions")  != t_json.end()) {
                extensions.clear();
                for (const auto &i : t_json["extensions"]) {
                    extensions.emplace_back(i.get<std::string>());
                }
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_data_pattern<misa_file_description>::to_json(t_json);
            t_json["filename"] = filename.string();
            {
                std::vector<std::string> extensions_;
                for(const auto &extension : extensions) {
                    extensions_.emplace_back(extension.string());
                }
                t_json["extensions"] = extensions_;
            }
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("filename").declare_optional<std::string>(filename.string());
            std::vector<std::string> extensions_;
            for(const auto &extension : extensions) {
                extensions_.emplace_back(extension.string());
            }
            t_schema.resolve("extensions").declare_optional<std::vector<std::string>>(extensions_);
        }

        bool has_filename() const {
            return !filename.empty();
        }

        bool has_extensions() const {
            return !extensions.empty();
        }

        bool matches(const boost::filesystem::path &t_path) const {
            for(const auto &extension : extensions) {
                if(t_path.extension() == extension)
                    return true;
            }
            return false;
        }

        misa_file_description produce() const {
            misa_file_description result;
            result.filename = filename;
            return result;
        }

        misa_file_description produce(const boost::filesystem::path &t_directory) const {
            misa_file_description result;
            if(has_filename()) {
                result.filename = filename;
            }
            else {
                for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_directory))) {
                    if(matches(entry.path())) {
                        result.filename = entry.path().filename();
                        break;
                    }
                }
            }
            return result;
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return misa_serializeable::create_serialization_id_hierarchy(misa_serialization_id("misa", "patterns/file"), {
                    misa_data_pattern<misa_file_description>::get_serialization_id_hierarchy()
            });
        }
    };

    inline void to_json(nlohmann::json& j, const misa_file_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_pattern& p) {
        p.from_json(j);
    }
}
