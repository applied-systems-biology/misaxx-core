#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

void misa_serializable::to_json(nlohmann::json &t_json) const {
    t_json["misa:serialization-hierarchy"] = get_serialization_id_hierarchy();
    t_json["misa:serialization-id"] = get_serialization_id().get_id();
}

void misa_serializable::to_json_schema(misa_json_schema_property &schema) const {
    schema.serialization_id = get_serialization_id();
    schema.serialization_hierarchy = get_serialization_id_hierarchy();
    std::string doc_name = get_documentation_name();
    std::string doc_description = get_documentation_description();
    if(!doc_name.empty()) {
        schema.document_title(doc_name);
        schema.document_type_title(std::move(doc_name));
    }
    if(!doc_description.empty()) {
        schema.document_description(doc_description);
        schema.document_type_description(std::move(doc_description));
    }
}

misa_serialization_id misa_serializable::get_serialization_id() const {
    const auto h = get_serialization_id_hierarchy();
    return h[h.size() - 1];
}

misa_serialization_id misa_serializable::get_serialization_id_from_json(const nlohmann::json &t_json) {
    return misa_serialization_id(t_json["misa:serialization-id"].get<std::string>());
}

misa_serialization_id
misa_serializable::get_serialization_id_from_json_or(const nlohmann::json &t_json, misa_serialization_id t_default) {
    if(t_json.is_object() && t_json.find("misa:serialization-id") != t_json.end()) {
        return get_serialization_id_from_json(t_json);
    }
    else {
        return t_default;
    }
}

std::vector<misa_serialization_id>
misa_serializable::get_serialization_hierarchy_from_json(const nlohmann::json &t_json) {
    return t_json["misa:serialization-hierarchy"].get<std::vector<misa_serialization_id>>();
}

std::vector<misa_serialization_id>
misa_serializable::get_serialization_hierarchy_from_json_or(const nlohmann::json &t_json,
                                                             std::vector<misa_serialization_id> t_default) {
    if(t_json.is_object() && t_json.find("misa:serialization-id") != t_json.end()) {
        return get_serialization_hierarchy_from_json(t_json);
    }
    else {
        return t_default;
    }
}

bool
misa_serializable::is_deserializable_from_json(const misa_serialization_id &target_id, const nlohmann::json &t_json) {
    if(!t_json.is_object())
        return false;
    const std::vector<misa_serialization_id> json_hierarchy = get_serialization_hierarchy_from_json_or(t_json, { });
    if(json_hierarchy.empty())
        return false;
    return std::find(json_hierarchy.begin(), json_hierarchy.end(), target_id) != json_hierarchy.end();
}

std::vector<misa_serialization_id> misa_serializable::get_serialization_id_hierarchy() const {
    std::vector<misa_serialization_id> result;
    build_serialization_id_hierarchy(result);
    return result;
}

void misa_serializable::from_json(const nlohmann::json &) { }

std::string misa_serializable::get_documentation_name() const {
    return "MISA Serializable";
}

std::string misa_serializable::get_documentation_description() const {
    return "JSON-serialized object";
}


