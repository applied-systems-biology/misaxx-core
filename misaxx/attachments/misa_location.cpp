//
// Created by rgerst on 05.12.18.
//

#include "misa_location.h"

using namespace misaxx;

misa_location::misa_location(boost::filesystem::path t_filesystem_location,
                             boost::filesystem::path t_filesystem_unique_location) :
        filesystem_location(std::move(t_filesystem_location)), filesystem_unique_location(std::move(t_filesystem_unique_location)) {

}

misa_location::misa_location(const misa_cached_data_base &t_cache) : filesystem_location(t_cache.get_cache_base()->get_location_in_filesystem()),
                                                                     filesystem_unique_location(t_cache.get_cache_base()->get_unique_location_in_filesystem()) {

}

void misa_location::from_json(const nlohmann::json &t_json) {
    filesystem_location = t_json["filesystem-location"].get<std::string>();
    filesystem_unique_location = t_json["filesystem-unique-location"].get<std::string>();
}

void misa_location::to_json(nlohmann::json &t_json) const {
    misa_serializeable::to_json(t_json);
    t_json["filesystem-location"] = filesystem_location.string();
    t_json["filesystem-unique-location"] = filesystem_unique_location.string();
}

void misa_location::to_json_schema(const misa_json_schema &t_schema) const {
    t_schema.resolve("filesystem-location").declare<std::string>();
    t_schema.resolve("filesystem-unique-location").declare<std::string>();
}

void misa_location::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializeable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "attachments/location"));
}
