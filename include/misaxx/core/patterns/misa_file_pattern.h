//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/core/misa_data_pattern.h>
#include <misaxx/core/descriptions/misa_file_description.h>

namespace misaxx {

    /**
     * A file pattern is a description that allows
     */
    struct misa_file_pattern : public misa_data_pattern {

        boost::filesystem::path filename;
        std::vector<boost::filesystem::path> extensions;

        misa_file_pattern() = default;

        explicit misa_file_pattern(std::vector<boost::filesystem::path> t_extensions);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

        bool has_filename() const;

        bool has_extensions() const;

        bool matches(const boost::filesystem::path &t_path) const;

        void apply(misa_file_description &target) const;

        void apply(misa_file_description &target, const boost::filesystem::path &t_directory) const;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_file_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_pattern& p) {
        p.from_json(j);
    }
}
