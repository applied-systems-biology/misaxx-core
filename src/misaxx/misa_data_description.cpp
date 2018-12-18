#include <misaxx/misa_data_description.h>

using namespace misaxx;

void misa_data_description::from_json(const nlohmann::json &t_json) {
}

void misa_data_description::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
}

void misa_data_description::to_json_schema(const misa_json_schema &t_schema) const {
}

void misa_data_description::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "descriptions/base"));
}
