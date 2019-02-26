//
// Created by rgerst on 05.02.19.
//

#include "misaxx/core/descriptions/misa_exported_attachments_description.h"

void misaxx::misa_exported_attachments_description::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_file_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa", "descriptions/exported-attachments"));
}

std::string misaxx::misa_exported_attachments_description::get_documentation_name() const {
    return "Exported attachments";
}

std::string misaxx::misa_exported_attachments_description::get_documentation_description() const {
    return "Contains a copy of all data attached to a cache";
}
