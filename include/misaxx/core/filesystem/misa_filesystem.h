//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <misaxx/core/filesystem/misa_filesystem_entry.h>

namespace misaxx {
    /**
     * Filesystem of a module
     * It has two folders "imported" and "exported"
     */
    struct misa_filesystem : public misa_serializable {
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
        bool is_valid() const;

        /**
         * Creates a sub-filesystem
         * @param t_name
         * @return
         */
        misa_filesystem create_subsystem(const std::string &t_name);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        std::shared_ptr<misa_filesystem_entry> find_external_path(const boost::filesystem::path &t_path) const;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };
}
