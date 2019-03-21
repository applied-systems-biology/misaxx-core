//
// Created by rgerst on 05.12.18.
//

#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/misa_cached_data_base.h>

using namespace misaxx;

misa_location::misa_location(boost::filesystem::path t_filesystem_location,
                             boost::filesystem::path t_filesystem_unique_location) :
        filesystem_location(std::move(t_filesystem_location)), filesystem_unique_location(std::move(t_filesystem_unique_location)) {

}

misa_location::misa_location(const misa_cached_data_base &t_cache) : filesystem_location(t_cache.get_cache_base()->get_location_in_filesystem()),
                                                                     filesystem_unique_location(t_cache.get_cache_base()->get_unique_location_in_filesystem()) {

}

void misa_location::from_json(const nlohmann::json &t_json) {
    internal_location = t_json["internal-location"].get<std::string>();
    filesystem_location = t_json["filesystem-location"].get<std::string>();
    filesystem_unique_location = t_json["filesystem-unique-location"].get<std::string>();
}

void misa_location::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    t_json["internal-location"] = internal_location.string();
    t_json["filesystem-location"] = filesystem_location.string();
    t_json["filesystem-unique-location"] = filesystem_unique_location.string();
}

void misa_location::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("internal-location")->declare_required<std::string>()
            .document_title("Internal Location")
            .document_description("Location within the MISA++ filesystem");
    t_schema.resolve("filesystem-location")->declare_required<std::string>()
            .document_title("Filesystem Location")
            .document_description("Location of the folder that contains the data");
    t_schema.resolve("filesystem-unique-location")->declare_required<std::string>()
            .document_title("Unique Filesystem Location")
            .document_description("File or folder within the data folder");
}

void misa_location::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "attachments/location"));
}

std::string misa_location::get_documentation_name() const {
    return "MISA Location";
}

std::string misa_location::get_documentation_description() const {
    return "Describes a location within data";
}
