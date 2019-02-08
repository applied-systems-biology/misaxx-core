//
// Created by rgerst on 05.11.18.
//


#pragma once

#include <misaxx/core/misa_serializable.h>

namespace misaxx {

    /**
     * A pattern consists of externally provided information how to import data.
     * During the import process, the pattern is analyzed and transformed into a description, which contains
     * all necessary data for additional processing.
     */
    struct misa_data_pattern : public misa_serializable {

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };
}
