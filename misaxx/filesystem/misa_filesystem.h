//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_filesystem_entry.h"
#include <pattxx/json/json_schema_builder.h>

namespace misaxx {
    /**
     * Filesystem of a MISA++ module. The filesystem has 3 folders, "imported", "exported" and "modules".
     */
    struct misa_filesystem {
        /**
         * Contains all imported data
         */
        filesystem::entry imported;
        /**
         * Contains all exported / output data
         */
        filesystem::entry exported;

        /**
         * Returns true if this filesystem is valid
         * @return
         */
        bool is_valid() const {
            return imported && exported;
        }

        /**
         * Creates a sub-filesystem
         * @param t_name
         * @return
         */
        misa_filesystem create_subsystem(const std::string &t_name) {
            if(!is_valid())
                throw std::runtime_error("Cannot create sub-filesystem from invalid filesystem!");
            misa_filesystem result;
            result.imported = imported->access(t_name);
            result.exported = exported->access(t_name);
            return result;
        }
    };

    inline pattxx::json::path_t json_child_path(pattxx::json::path_t t_path, std::string t_segment) {
        t_path.emplace_back(std::move(t_segment));
        return t_path;
    }

    inline void to_json_schema(const filesystem::const_entry &t_entry, const pattxx::json::path_t &t_path, pattxx::json::json_schema_builder &t_builder) {
        t_builder.insert_optional<std::string>(json_child_path(t_path, "external-path"), "", pattxx::json::json_property<std::string>());
//        if(!t_entry->metadata->is_empty()) {
//            to_json_schema(t_builder, json_child_path(t_path, "data-metadata"), *t_entry->metadata);
//        }

        // TODO: Patterns & descriptions

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
