//
// Created by rgerst on 05.11.18.
//


#pragma once

#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/misa_json_schema_property.h>

namespace misaxx {

    /**
     * A pattern is used to produce or setup a misaxx::misa_data_description.
     * It should contain information and methods to extract the correct files that contain the actual data.
     */
    struct misa_data_pattern : public misa_serializable {

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };
}
