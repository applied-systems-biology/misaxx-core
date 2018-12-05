//
// Created by rgerst on 05.12.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>
#include <misaxx/misa_cached_data_base.h>

namespace misaxx {

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

        explicit misa_location(boost::filesystem::path t_filesystem_location, boost::filesystem::path t_filesystem_unique_location) :
            filesystem_location(std::move(t_filesystem_location)), filesystem_unique_location(std::move(t_filesystem_unique_location)) {

        }

        /**
         * Initializes the location from cached data
         * @param t_cache
         */
        explicit misa_location(const misa_cached_data_base &t_cache) : filesystem_location(t_cache.get_cache_base()->get_location_in_filesystem()),
            filesystem_unique_location(t_cache.get_cache_base()->get_unique_location_in_filesystem()) {

        }

        void from_json(const nlohmann::json &t_json) override {
            filesystem_location = t_json["filesystem-location"].get<std::string>();
            filesystem_unique_location = t_json["filesystem-unique-location"].get<std::string>();
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json["filesystem-location"] = filesystem_location.string();
            t_json["filesystem-unique-location"] = filesystem_unique_location.string();
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("filesystem-location").declare<std::string>();
            t_schema.resolve("filesystem-unique-location").declare<std::string>();
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "attachments/location");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return { get_serialization_id() };
        }
    };

    inline void to_json(nlohmann::json& j, const misa_location& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_location& p) {
        p.from_json(j);
    }

}



