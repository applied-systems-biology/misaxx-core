//
// Created by rgerst on 26.02.19.
//

#include "misaxx/core/descriptions/misa_folder_description.h"

void misaxx::misa_folder_description::from_json(const nlohmann::json &t_json) {
    misa_data_description::from_json(t_json);
    folder = t_json["folder"].get<boost::filesystem::path>();
}

void misaxx::misa_folder_description::to_json(nlohmann::json &t_json) const {
    misa_data_description::to_json(t_json);
    t_json["folder"] = folder;
}

void misaxx::misa_folder_description::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_data_description::to_json_schema(t_schema);
    t_schema["folder"].declare_required<boost::filesystem::path>();
}

void misaxx::misa_folder_description::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_data_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "descriptions/folder"));
}

std::string misaxx::misa_folder_description::get_documentation_name() const {
    return "Folder";
}

std::string misaxx::misa_folder_description::get_documentation_description() const {
    return "A folder within the filesystem";
}
