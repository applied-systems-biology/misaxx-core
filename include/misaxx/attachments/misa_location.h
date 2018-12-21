//
// Created by rgerst on 05.12.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {

    struct misa_cached_data_base;

    /**
     * Allows finding an object within the filesystem.
     * External programs can look for this type in the serialization hierarchy.
     * All location providers should inherit from this attachment
     */
    struct misa_location : public misa_serializeable {

        /**
         * Internal path within the filesystem that contains the referenced data
         * a valid path begins with either "imported" or "exported"
         */
        boost::filesystem::path filesystem_location;

        /**
         * An optional additional internal path that uniquely identifies sub-caches within the same location
         */
        boost::filesystem::path filesystem_unique_location;

        misa_location() = default;

        explicit misa_location(boost::filesystem::path t_filesystem_location, boost::filesystem::path t_filesystem_unique_location);

        /**
         * Initializes the location from cached data
         * @param t_cache
         */
        explicit misa_location(const misa_cached_data_base &t_cache);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

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



