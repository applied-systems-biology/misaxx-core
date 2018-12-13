#include <misaxx/patterns/misa_file_stack_pattern.h>

using namespace misaxx;

misa_file_stack_pattern::misa_file_stack_pattern(std::vector<boost::filesystem::path> t_extensions) : extensions(std::move(t_extensions)) {

}

misa_file_stack_description misa_file_stack_pattern::produce(const boost::filesystem::path &t_directory) const {
    misa_file_stack_description result;
    for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_directory))) {
        for(const auto &extension : extensions) {
            if(boost::iequals(entry.path().extension().string(), extension.string())) {
                result.files.insert({ entry.path().filename().string(), misa_file_description(entry.path()) });
                break;
            }
        }
    }
    return result;
}

void misa_file_stack_pattern::from_json(const nlohmann::json &t_json) {
    extensions.clear();
    for (const auto &i : t_json["extensions"]) {
        extensions.emplace_back(i.get<std::string>());
    }
}

void misa_file_stack_pattern::to_json(nlohmann::json &t_json) const {
    misa_data_pattern<misa_file_stack_description>::to_json(t_json);
    std::vector<std::string> extensions_;
    for(const auto &extension : extensions) {
        extensions_.emplace_back(extension.string());
    }
    t_json["extensions"] = extensions_;
}

void misa_file_stack_pattern::to_json_schema(const misa_json_schema &t_schema) const {
    std::vector<std::string> extensions_;
    for(const auto &extension : extensions) {
        extensions_.emplace_back(extension.string());
    }
    t_schema.resolve("extensions").declare_optional<std::vector<std::string>>(extensions_);
}

void misa_file_stack_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_pattern_base::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/file-stack"));
}
