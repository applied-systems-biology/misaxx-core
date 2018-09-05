//
// Created by rgerst on 05.09.18.
//


#pragma once

#include <pattxx/metadata.h>
#include <pattxx/json/metadata_serializer.h>
#include <nlohmann/json.hpp>
#include "../object_node_path.h"

namespace misaxx {
    struct object_metadata : public pattxx::metadata {
        using configuration_namespace_type = object_node_path;
        static inline const std::string name = "name";
        static inline const pattxx::metadata metadata = pattxx::metadata("Object name");
        using pattxx::metadata::metadata;
    };
}

namespace nlohmann {

    /**
     * Serializes a boost::filesystem::path
     */
    template<> struct adl_serializer<misaxx::object_metadata> {
        static void to_json(json& j, const misaxx::object_metadata& opt) {
            if(opt.description.empty()) {
                j = opt.title;
            }
            else {
                j["title"] = opt.title;
                j["description"] = opt.description;
            }
        }

        static void from_json(const json& j, misaxx::object_metadata& opt) {
            if(j.is_string()) {
                opt.title = j;
            }
            else {
                opt.title = j["title"];
                opt.description = j["description"];
            }
        }
    };

}