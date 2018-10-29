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
            if(t_json.find("filename") != t_json.end())
                filename = t_json["filename"];
            if(t_json.find("filetype")  != t_json.end())
                filetype = t_json["filetype"];
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["filename"] = filename;
            t_json["filetype"] = filetype;
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
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_file_description& p) {
        p.from_json(j);
    }
}
