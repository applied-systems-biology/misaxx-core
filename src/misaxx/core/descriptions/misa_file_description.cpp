#include <misaxx/core/descriptions/misa_file_description.h>
#include <misaxx/core/misa_json_schema_property.h>

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

void misa_file_description::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_data_description::to_json_schema(t_schema);
    t_schema.resolve("filename")->declare_required<std::string>()
            .document_title("Filename")
            .document_description("The filename");
}

void misa_file_description::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "descriptions/file"));
}

std::string misa_file_description::get_documentation_name() const {
    return "File";
}

std::string misa_file_description::get_documentation_description() const {
    return "A file located within the filesystem";
}
