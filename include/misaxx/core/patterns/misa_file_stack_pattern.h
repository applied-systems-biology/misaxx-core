//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/core/descriptions/misa_file_stack_description.h>
#include <misaxx/core/patterns/misa_file_pattern.h>
#include <misaxx/core/misa_data_pattern.h>

namespace misaxx {
    struct misa_file_stack_pattern : public misa_data_pattern<misa_file_stack_description> {

        std::vector<boost::filesystem::path> extensions;

        misa_file_stack_pattern() = default;

        explicit misa_file_stack_pattern(std::vector<boost::filesystem::path> t_extensions);

        misa_file_stack_description produce(const boost::filesystem::path &t_directory) const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_file_stack_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_stack_pattern& p) {
        p.from_json(j);
    }
}
