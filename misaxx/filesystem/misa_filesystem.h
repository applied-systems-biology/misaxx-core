//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <misaxx/filesystem/misa_filesystem_entry.h>

namespace misaxx {
    /**
     * Filesystem of a MISA++ module. The filesystem has 3 folders, "imported", "exported" and "modules".
     */
    struct misa_filesystem : public misa_serializeable {
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

        void from_json(const nlohmann::json &t_json) override {
            throw std::runtime_error("Not implemented");
        }

        void to_json(nlohmann::json &t_json) const override {
            throw std::runtime_error("Not implemented");
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            imported->to_json_schema(t_schema.resolve("imported"));
            exported->to_json_schema(t_schema.resolve("exported"));
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "filesystem");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return { get_serialization_id() };
        }
    };
}
