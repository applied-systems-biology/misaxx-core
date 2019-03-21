//
// Created by rgerst on 05.12.18.
//

#pragma once

#include <misaxx/core/misa_serializable.h>

namespace misaxx {

    struct misa_cached_data_base;

    /**
     * Base type for custom locations that identifies the location within the filesystem.
     */
    struct misa_location : public misa_serializable {

        /**
         * Internal path of the data
         */
        boost::filesystem::path internal_location;

        /**
         * Path to the folder that contains the data
         */
        boost::filesystem::path filesystem_location;

        /**
         * An optional additional sub-path within the filesystem location
         */
        boost::filesystem::path filesystem_unique_location;

        misa_location() = default;

        explicit misa_location(boost::filesystem::path t_internal_location, boost::filesystem::path t_filesystem_location, boost::filesystem::path t_filesystem_unique_location);

        /**
         * Initializes the location from cached data
         * @param t_cache
         */
        explicit misa_location(const misa_cached_data_base &t_cache);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_location& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_location& p) {
        p.from_json(j);
    }

}



