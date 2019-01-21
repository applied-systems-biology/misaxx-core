#include <misaxx/core/json/misa_json_schema_builder.h>

using namespace misaxx;

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

void misa_json_schema_builder::write(const boost::filesystem::path &t_path) {
    std::ofstream out;
    out.open(t_path.string());
    out << std::setw(4) << data;
    out.close();
}
