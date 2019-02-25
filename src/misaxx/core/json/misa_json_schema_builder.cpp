#include <misaxx/core/json/misa_json_schema_builder.h>

using namespace misaxx;

misa_json_schema_builder::misa_json_schema_builder() : data({ {"type", "object"}, {"$schema", "http://json-schema.org/draft-07/schema#"} }) {

}


misa_json_schema_builder::path_t
misa_json_schema_builder::schema_property_path(const misa_json_schema_builder::path_t &t_parameter_path) {
    path_t base_path;
    for(size_t i = 0; i < t_parameter_path.size(); ++i) {
        base_path.emplace_back("properties");
        base_path.push_back(t_parameter_path[i]);
    }
    return base_path;
}

misa_json_schema_builder::path_t
misa_json_schema_builder::schema_parent_path(const misa_json_schema_builder::path_t &t_parameter_path) {
    path_t base_path;
    for(size_t i = 0; i < t_parameter_path.size() - 1; ++i) {
        base_path.emplace_back("properties");
        base_path.push_back(t_parameter_path[i]);
    }
    return base_path;
}

void misa_json_schema_builder::ensure_schema_property_path(const misa_json_schema_builder::path_t &t_parameter_path) {
    path_t base_path;
    for(size_t i = 0; i < t_parameter_path.size() - 1; ++i) {
        base_path.emplace_back("properties");
        base_path.push_back(t_parameter_path[i]);

        // Set the pointer accordingly
        json_helper::access_json_path(data, base_path, "type") = "object";
    }
}

void misa_json_schema_builder::write(const boost::filesystem::path &t_path) const {
    std::ofstream out;
    out.open(t_path.string());
    out << std::setw(4) << data;
    out.close();
}

void misa_json_schema_builder::insert_common(const misa_json_schema_builder::path_t &t_parameter_path,
                                             const misa_json_property_base &t_json_metadata) {
    const auto property_base_path = schema_property_path(t_parameter_path);
    const auto property_parent_base_path = schema_parent_path(t_parameter_path);

    // If the property is required, update the required list
    if(t_json_metadata.required) {
        nlohmann::json &required_list = json_helper::access_json_path(data, property_parent_base_path, "required");

        if(required_list.empty()) {
            required_list = { t_parameter_path[t_parameter_path.size() - 1] };
        }
        else {
            const auto &item = t_parameter_path[t_parameter_path.size() - 1];
            for(const auto &v : required_list) {
                if(v == item)
                    return;
            }
            required_list.push_back(item);
        }
    }

    // Add documentation if available
    if(!t_json_metadata.title.empty()) {
        annotate(t_parameter_path, "misa:documentation-title", t_json_metadata.title);
    }

    if(!t_json_metadata.description.empty()) {
        annotate(t_parameter_path, "misa:documentation-description", t_json_metadata.description);
    }
}

