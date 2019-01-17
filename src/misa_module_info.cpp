//
// Created by rgerst on 16.01.19.
//

#include <misaxx/misa_module_info.h>

using namespace misaxx;

std::string misa_module_info::get_name() const {
    return m_name;
}

std::string misa_module_info::get_version() const {
    return m_version;
}

std::string misa_module_info::get_description() const {
    if(m_description.empty())
        return get_name();
    else
        return m_description;
}

std::vector<misa_module_info> misa_module_info::get_dependencies() const {
    return m_dependencies;
}

void misa_module_info::from_json(const nlohmann::json &t_json) {
    m_name = t_json["name"];
    m_version = t_json["version"];
    if(t_json.find("description") != t_json.end())
        m_description = t_json["description"];
    if(t_json.find("dependencies") != t_json.end())
        m_dependencies = t_json["dependencies"].get<std::vector<misa_module_info>>();
}

void misa_module_info::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
    t_json["name"] = m_name;
    t_json["version"] = m_version;
    t_json["description"] = get_description();
    t_json["dependencies"] = m_dependencies;
}

void misa_module_info::to_json_schema(const misa_json_schema &t_schema) const {
    misa_serializeable::to_json_schema(t_schema);
    t_schema.resolve("name").declare_required<std::string>();
    t_schema.resolve("version").declare_required<std::string>();
    t_schema.resolve("description").declare_optional<std::string>();
    t_schema.resolve("dependencies").declare_optional<std::vector<misa_module_info>>();
}

void misa_module_info::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "module-info"));
}



