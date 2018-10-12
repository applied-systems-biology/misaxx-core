//
// Created by rgerst on 05.10.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <pattxx/json/json_schema_builder.h>
#include "misaxx/misa_filesystem.h"

namespace misaxx::filesystem {

    inline pattxx::json::path_t json_child_path(pattxx::json::path_t t_path, std::string t_segment) {
        t_path.emplace_back(std::move(t_segment));
        return t_path;
    }

    inline void to_json_schema(const const_file &t_file, const pattxx::json::path_t &t_path, pattxx::json::json_schema_builder &t_builder) {
        t_builder.insert_static<std::string>(json_child_path(t_path, "type"), "file",
                pattxx::json::json_property<std::string>("type", "Type of the filesystem node"));
        t_builder.insert_optional<std::string>(json_child_path(t_path, "external-path"), "",
                pattxx::json::json_property<std::string>("external-path", "External path of VFS file"));
        if(!t_file->data_string.empty()) {
            t_builder.insert_static<std::string>(json_child_path(t_path, "data-type"), t_file->data_string,
                                                 pattxx::json::json_property<std::string>("data-type", "How this filesystem node is interpreted by the MISA module"));
        }
    }

    inline void to_json_schema(const const_folder &t_folder, const pattxx::json::path_t &t_path, pattxx::json::json_schema_builder &t_builder) {

        t_builder.insert_static<std::string>(json_child_path(t_path, "type"), "folder",
                pattxx::json::json_property<std::string>("type", "Type of the filesystem node"));
        t_builder.insert_optional<std::string>(json_child_path(t_path, "external-path"), "",
                pattxx::json::json_property<std::string>("external-path", "External path of VFS folder"));
        if(!t_folder->data_string.empty()) {
            t_builder.insert_static<std::string>(json_child_path(t_path, "data-type"), t_folder->data_string,
                                                 pattxx::json::json_property<std::string>("data-type", "How this filesystem node is interpreted by the MISA module"));
        }

        if(!t_folder->empty()) {
            for(const auto &kv : *t_folder) {
                if(kv.second->is_folder()) {
                    auto new_path = t_path;
                    new_path.emplace_back("children");
                    new_path.emplace_back(kv.first);
                    to_json_schema(std::dynamic_pointer_cast<const vfs_folder>(kv.second), new_path, t_builder);
                }
                else if(kv.second->is_file()) {
                    auto new_path = t_path;
                    new_path.emplace_back("children");
                    new_path.emplace_back(kv.first);
                    to_json_schema(std::dynamic_pointer_cast<const vfs_file>(kv.second), new_path, t_builder);
                }
                else {
                    throw std::runtime_error("Unknown vfs node type!");
                }
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