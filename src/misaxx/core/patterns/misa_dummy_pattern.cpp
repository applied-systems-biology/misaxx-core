//
// Created by rgerst on 27.11.18.
//

#include <misaxx/core/patterns/misa_dummy_pattern.h>

std::string misaxx::misa_dummy_pattern::get_documentation_name() const {
    return "No pattern";
}

std::string misaxx::misa_dummy_pattern::get_documentation_description() const {
    return "Unused pattern";
}

void
misaxx::misa_dummy_pattern::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_data_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/dummy"));
}
