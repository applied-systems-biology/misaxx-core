//
// Created by rgerst on 26.10.18.
//


#pragma once

#include <misaxx/misa_serializeable.h>
#include <misaxx/misa_data_description.h>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>

namespace misaxx {

    /**
     * Describes a file
     */
    struct misa_file_description : public misa_data_description {

        boost::filesystem::path filename;

        misa_file_description() = default;

        explicit misa_file_description(const boost::filesystem::path &t_path) : filename(t_path.filename()) {

        }

        void from_json(const nlohmann::json &t_json) override {
            filename = t_json["filename"].get<std::string>();
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            t_json["filename"] = filename.string();
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("filename").declare_required<std::string>();
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "descriptions/file");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return create_serialization_id_hierarchy(*this, { misa_data_description::get_serialization_id_hierarchy() });
        }
    };

    inline void to_json(nlohmann::json& j, const misa_file_description& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_description& p) {
        p.from_json(j);
    }
}
