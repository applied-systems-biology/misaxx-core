#include <misaxx/descriptions/misa_file_description.h>

using namespace misaxx;

misa_file_description::misa_file_description(const boost::filesystem::path &t_path) : filename(t_path.filename()) {

}

void misa_file_description::from_json(const nlohmann::json &t_json) {
    filename = t_json["filename"].get<std::string>();
}

void misa_file_description::to_json(nlohmann::json &t_json) const {
    misa_data_description::to_json(t_json);
    t_json["filename"] = filename.string();
}

void misa_file_description::to_json_schema(const misa_json_schema &t_schema) const {
    misa_data_description::to_json_schema(t_schema);
    t_schema.resolve("filename").declare_required<std::string>();
}

void misa_file_description::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "descriptions/file"));
}
