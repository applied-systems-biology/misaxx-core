//
// Created by rgerst on 11.12.18.
//

#include "misaxx/attachments/misa_unit_numeric.h"

using namespace misaxx;

void misa_unit_numeric::from_json(const nlohmann::json &) {
}

void misa_unit_numeric::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
}

void misa_unit_numeric::to_json_schema(const misa_json_schema &) const {
}

std::string misa_unit_numeric::get_literal() const {
    return ""; // No literal
}

bool misa_unit_numeric::operator==(const misa_unit_numeric &) const {
    return true;
}

bool misa_unit_numeric::operator!=(const misa_unit_numeric &) const {
    return false;
}

void misa_unit_numeric::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "attachments/quantities/numeric"));
}
