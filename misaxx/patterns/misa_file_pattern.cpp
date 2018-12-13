#include <misaxx/patterns/misa_file_pattern.h>

using namespace misaxx;

misa_file_pattern::misa_file_pattern(std::vector<boost::filesystem::path> t_extensions) :
        extensions(std::move(t_extensions)) {

}

void misa_file_pattern::from_json(const nlohmann::json &t_json) {
    if(t_json.find("filename") != t_json.end())
        filename = t_json["filename"].get<std::string>();
    if(t_json.find("extensions")  != t_json.end()) {
        extensions.clear();
        for (const auto &i : t_json["extensions"]) {
            extensions.emplace_back(i.get<std::string>());
        }
    }
}

void misa_file_pattern::to_json(nlohmann::json &t_json) const {
    misa_data_pattern<misa_file_description>::to_json(t_json);
    t_json["filename"] = filename.string();
    {
        std::vector<std::string> extensions_;
        for(const auto &extension : extensions) {
            extensions_.emplace_back(extension.string());
        }
        t_json["extensions"] = extensions_;
    }
}

void misa_file_pattern::to_json_schema(const misa_json_schema &t_schema) const {
    t_schema.resolve("filename").declare_optional<std::string>(filename.string());
    std::vector<std::string> extensions_;
    for(const auto &extension : extensions) {
        extensions_.emplace_back(extension.string());
    }
    t_schema.resolve("extensions").declare_optional<std::vector<std::string>>(extensions_);
}

bool misa_file_pattern::has_filename() const {
    return !filename.empty();
}

bool misa_file_pattern::has_extensions() const {
    return !extensions.empty();
}

bool misa_file_pattern::matches(const boost::filesystem::path &t_path) const {
    for(const auto &extension : extensions) {
        if(t_path.extension() == extension)
            return true;
    }
    return false;
}

misa_file_description misa_file_pattern::produce() const {
    misa_file_description result;
    result.filename = filename;
    return result;
}

misa_file_description misa_file_pattern::produce(const boost::filesystem::path &t_directory) const {
    misa_file_description result;
    if(has_filename()) {
        result.filename = filename;
    }
    else {
        for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_directory))) {
            if(matches(entry.path())) {
                result.filename = entry.path().filename();
                break;
            }
        }
    }
    return result;
}

void misa_file_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_pattern_base::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/file"));
}
