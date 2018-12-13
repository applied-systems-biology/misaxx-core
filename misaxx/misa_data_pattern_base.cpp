//
// Created by rgerst on 05.11.18.
//

#include "misa_data_pattern_base.h"

using namespace misaxx;

void misa_data_pattern_base::build_serialization_id_hierarchy(
        std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/base"));
}

void misa_data_pattern_base::to_json_schema(const misa_json_schema &t_schema) const {
}

void misa_data_pattern_base::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
}

void misa_data_pattern_base::from_json(const nlohmann::json &t_json) {
}
