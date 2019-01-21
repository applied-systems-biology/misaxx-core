//
// Created by rgerst on 21.12.18.
//

#include <misaxx/core/attachments/misa_labeled_object_location.h>

using namespace misaxx;

misa_labeled_object_location::misa_labeled_object_location(const misa_cached_data_base &t_cache, int t_label) :
misa_location(t_cache), label(t_label) {

}

void misa_labeled_object_location::from_json(const nlohmann::json &t_json) {
    misa_location::from_json(t_json);
    label = t_json["label"].get<int>();
}

void misa_labeled_object_location::to_json(nlohmann::json &t_json) const {
    misa_location::to_json(t_json);
    t_json["label"] = label;
}

void misa_labeled_object_location::to_json_schema(const misa_json_schema &t_schema) const {
    misa_location::to_json_schema(t_schema);
    t_schema.resolve("label").declare_required<int>();
}

void misa_labeled_object_location::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_location::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "attachments/labeled-object-location"));
}


