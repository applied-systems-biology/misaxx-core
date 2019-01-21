#include <misaxx/core/misa_description_storage.h>

using namespace misaxx;

misa_description_storage::misa_description_storage(const misa_description_storage &t_source) {
    nlohmann::json json;
    t_source.to_json(json);
    from_json(json);
}

std::shared_ptr<misa_description_storage> misa_description_storage::clone() const {
    return std::make_shared<misa_description_storage>(*this);
}

void misa_description_storage::from_imported_json(const nlohmann::json &t_json) {
    if(t_json.find("pattern") != t_json.end() || t_json.find("description") != t_json.end()) {
        from_json(t_json);
    }
    else {
        m_raw_pattern_json = t_json;
    }
}

void misa_description_storage::from_json(const nlohmann::json &t_json) {
    {
        auto it = t_json.find("pattern");
        if(it != t_json.end()) {
            m_raw_pattern_json = it.value();
        }
    }
    {
        auto it = t_json.find("description");
        if(it != t_json.end()) {
            m_raw_description_json = it.value();
        }
    }
}

void misa_description_storage::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
    t_json["pattern"] = m_raw_pattern_json; // Pass along the raw metadata. This is very important!
    t_json["description"] = m_raw_description_json;

    if(static_cast<bool>(m_pattern)) {
        auto &j = t_json["pattern"];
        m_pattern->to_json(j);
    }
    if(static_cast<bool>(m_description)) {
        auto &j = t_json["description"];
        m_description->to_json(j);
    }
}

void misa_description_storage::to_json_schema(const misa_json_schema &t_schema) const {
    misa_serializeable::to_json_schema(t_schema);
    if(has_pattern()) {
        t_schema.resolve("pattern", "pattern-type").define(get<misa_data_pattern_base>().get_serialization_id());
        get<misa_data_pattern_base>().to_json_schema(t_schema.resolve("pattern"));
    }
    if(has_description()) {
        t_schema.resolve("description", "description-type").define(get<misa_data_description>().get_serialization_id());
        get<misa_data_description>().to_json_schema(t_schema.resolve("description"));
    }
}

void misa_description_storage::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "description-storage"));
}

bool misa_description_storage::has_pattern() const {
    return static_cast<bool>(m_pattern) ||
           misa_serializeable::type_is_deserializeable_from_json<misa_data_pattern_base>(m_raw_pattern_json);
}

bool misa_description_storage::has_description() const {
    return static_cast<bool>(m_description) ||
           misa_serializeable::type_is_deserializeable_from_json<misa_data_description>(m_raw_description_json);
}

misa_description_storage &misa_description_storage::operator=(const misa_description_storage &t_source) {
    nlohmann::json json;
    t_source.to_json(json);
    from_json(json);
    return *this;
}
