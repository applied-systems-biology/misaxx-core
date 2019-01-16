//
// Created by rgerst on 16.01.19.
//

#include <misaxx/misa_module_info.h>

#include "misaxx/misa_module_info.h"

using namespace misaxx;

misa_module_info::misa_module_info(std::string t_name, std::string t_version) : m_name(std::move(t_name)), m_version(std::move(t_version)) {

}

std::string misa_module_info::get_name() const {
    return m_name;
}

std::string misa_module_info::get_version() const {
    return m_version;
}

void misa_module_info::from_json(const nlohmann::json &t_json) {
    m_name = t_json["name"];
    m_version = t_json["version"];
}

void misa_module_info::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
    t_json["name"] = m_name;
    t_json["version"] = m_version;
}

void misa_module_info::to_json_schema(const misa_json_schema &t_schema) const {
    misa_serializeable::to_json_schema(t_schema);
    t_schema.resolve("name").declare_required<std::string>();
    t_schema.resolve("version").declare_required<std::string>();
}

void misa_module_info::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "module-info"));
}
