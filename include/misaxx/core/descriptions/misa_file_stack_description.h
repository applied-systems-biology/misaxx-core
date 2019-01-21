//
// Created by rgerst on 26.10.18.
//


#pragma once

#include <misaxx/core/misa_serializeable.h>
#include <misaxx/core/descriptions/misa_file_description.h>
#include <misaxx/core/misa_data_description.h>

namespace misaxx {
    struct misa_file_stack_description : public misa_data_description {

        using files_type = std::unordered_map<std::string, misa_file_description>;

        files_type files;

        misa_file_stack_description() = default;

        explicit misa_file_stack_description(files_type t_files);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    };

    inline void to_json(nlohmann::json& j, const misa_file_stack_description& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_stack_description& p) {
        p.from_json(j);
    }
}
