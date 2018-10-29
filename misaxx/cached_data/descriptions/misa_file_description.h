//
// Created by rgerst on 26.10.18.
//


#pragma once

#include "../../misa_metadata.h"
#include <boost/filesystem.hpp>

namespace misaxx {
    struct misa_file_description : public misa_metadata {

        std::string filename;
        std::string filetype;

        void from_json(const nlohmann::json &t_json) override {
            filename = t_json["filename"];
            filetype = t_json["filetype"];

            // Auto-assign filetype from filename if needed
            if(!filename.empty() && filetype.empty()) {
                filetype = boost::filesystem::path(filename).extension().string();
            }
        }

        nlohmann::json to_json() const override {
            nlohmann::json result;
            result["filename"] = filename;
            result["filetype"] = filetype;
            return result;
        }

        std::string get_name() const override {
            return "misa-file";
        }

        bool has_filename() const {
            return !filename.empty();
        }

        bool has_filetype() const {
            return !filetype.empty();
        }
    };

    void to_json(nlohmann::json& j, const misa_file_description& p) {
        j = p.to_json();
    }

    void from_json(const nlohmann::json& j, misa_file_description& p) {
        p.from_json(j);
    }
}
