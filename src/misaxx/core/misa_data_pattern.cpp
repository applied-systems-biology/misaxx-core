//
// Created by rgerst on 05.11.18.
//

#include <misaxx/core/misa_data_pattern.h>

using namespace misaxx;

void misa_data_pattern::build_serialization_id_hierarchy(
        std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/base"));
}

void misa_data_pattern::to_json_schema(const misa_json_schema &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
}

void misa_data_pattern::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
}

void misa_data_pattern::from_json(const nlohmann::json &) {
}
