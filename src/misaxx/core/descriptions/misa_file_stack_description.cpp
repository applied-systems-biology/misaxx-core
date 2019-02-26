#include <misaxx/core/descriptions/misa_file_stack_description.h>
#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

misa_file_stack_description::misa_file_stack_description(misa_file_stack_description::files_type t_files) : files(std::move(t_files)) {

}

void misa_file_stack_description::from_json(const nlohmann::json &t_json) {
    if(t_json.find("files") != t_json.end()) {
        for(auto it = t_json["files"].begin(); it != t_json["files"].end(); ++it) {
            files[it.key()] = it.value();
        }
    }
}

void misa_file_stack_description::to_json(nlohmann::json &t_json) const {
    misa_data_description::to_json(t_json);
    t_json["files"] = files;
}

void misa_file_stack_description::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_data_description::to_json_schema(t_schema);
    for(const auto &kv : files) {
        kv.second.to_json_schema(t_schema["files"][kv.first]);
    }
}

void misa_file_stack_description::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "descriptions/file-stack"));
}

std::string misa_file_stack_description::get_documentation_name() const {
    return "File stack";
}

std::string misa_file_stack_description::get_documentation_description() const {
    return "A list of files within the filesystem";
}
