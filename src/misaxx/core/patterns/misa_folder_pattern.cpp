//
// Created by rgerst on 26.02.19.
//

#include <misaxx/core/patterns/misa_folder_pattern.h>

#include "misaxx/core/patterns/misa_folder_pattern.h"

void misaxx::misa_folder_pattern::from_json(const nlohmann::json &t_json) {
    misa_data_pattern::from_json(t_json);
}

void misaxx::misa_folder_pattern::to_json(nlohmann::json &t_json) const {
    misa_data_pattern::to_json(t_json);
}

void misaxx::misa_folder_pattern::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_data_pattern::to_json_schema(t_schema);
}

void misaxx::misa_folder_pattern::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_data_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/folder"));
}

void misaxx::misa_folder_pattern::apply(misaxx::misa_folder_description &target,
                                        const boost::filesystem::path &t_directory) const {
    target.folder = t_directory;
}
