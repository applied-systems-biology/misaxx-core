//
// Created by rgerst on 26.10.18.
//


#pragma once

#include "../../misa_metadata.h"
#include "../misa_description.h"
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>

namespace misaxx {

    /**
     * Describes a file
     */
    struct misa_file_description : public misa_description {

        boost::filesystem::path filename;

        misa_file_description() = default;

        explicit misa_file_description(const boost::filesystem::path &t_path) : filename(t_path.filename()) {

        }

        void from_json(const nlohmann::json &t_json) override {
            filename = t_json["filename"].get<std::string>();
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["filename"] = filename.string();
        }
    };

    void to_json(nlohmann::json& j, const misa_file_description& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_file_description& p) {
        p.from_json(j);
    }
}
