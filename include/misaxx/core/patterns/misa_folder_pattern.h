//
// Created by rgerst on 26.02.19.
//

#pragma once

#include <misaxx/core/misa_data_pattern.h>
#include <misaxx/core/descriptions/misa_folder_description.h>

namespace misaxx {
    struct misa_folder_pattern : public misa_data_pattern {
        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        /**
         * Applies the pattern and
         * sets the filename of the description
         * @param target
         * @param t_directory
         */
        virtual void apply(misa_folder_description &target, const boost::filesystem::path &t_directory) const;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_folder_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_folder_pattern& p) {
        p.from_json(j);
    }
}




