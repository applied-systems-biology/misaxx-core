//
// Created by rgerst on 01.11.18.
//


#pragma once

#include "../misa_pattern.h"
#include "../descriptions/misa_file_description.h"

namespace misaxx {

    /**
     * A file pattern is a description that allows
     */
    struct misa_file_pattern : public misa_pattern<misa_file_description> {

        boost::filesystem::path filename;
        boost::filesystem::path extension;

        misa_file_pattern() = default;

        void from_json(const nlohmann::json &t_json) override {
            if(t_json.find("filename") != t_json.end())
                filename = t_json["filename"].get<std::string>();
            if(t_json.find("extension")  != t_json.end())
                extension = t_json["extension"].get<std::string>();
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["filename"] = filename.string();
            t_json["extension"] = extension.string();
        }

        bool has_filename() const {
            return !filename.empty();
        }

        bool has_extension() const {
            return !extension.empty();
        }

        bool matches_filetype(const boost::filesystem::path &t_path) const {
            return t_path.extension() == extension;
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
                    if(boost::iequals(entry.path().extension().string(), extension.string())) {
                        result.filename = entry.path().filename();
                        break;
                    }
                }
            }
            return result;
        }
    };

    void to_json(nlohmann::json& j, const misa_file_pattern& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_file_pattern& p) {
        p.from_json(j);
    }
}
