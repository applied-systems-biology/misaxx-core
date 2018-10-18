//
// Created by rgerst on 05.10.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <pattxx/json/json_schema_builder.h>
#include "misa_filesystem.h"

namespace misaxx::filesystem {

    inline pattxx::json::path_t json_child_path(pattxx::json::path_t t_path, std::string t_segment) {
        t_path.emplace_back(std::move(t_segment));
        return t_path;
    }

    inline void to_json_schema(const const_entry &t_entry, const pattxx::json::path_t &t_path, pattxx::json::json_schema_builder &t_builder) {
        t_builder.insert_optional<std::string>(json_child_path(t_path, "external-path"), "", pattxx::json::json_property<std::string>());
        if(!t_entry->data_string.empty()) {
            t_builder.insert_static<std::string>(json_child_path(t_path, "data-type"), t_entry->data_string, pattxx::json::json_property<std::string>());
        }

        if(!t_entry->empty()) {
            for(const auto &kv : *t_entry) {
                auto new_path = t_path;
                new_path.emplace_back("children");
                new_path.emplace_back(kv.first);
                to_json_schema(kv.second, new_path, t_builder);
            }
        }
    }

    /**
     * Converts the filesystem into a JSON schema
     * @param t_filesystem
     * @param t_builder
     */
    inline void to_json_schema(const misa_filesystem &t_filesystem, pattxx::json::json_schema_builder &t_builder) {
        to_json_schema(t_filesystem.imported, {"filesystem", "json-data", "imported"}, t_builder);
        to_json_schema(t_filesystem.exported, {"filesystem", "json-data", "exported"}, t_builder);
    }
}