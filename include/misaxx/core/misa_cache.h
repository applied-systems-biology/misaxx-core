//
// Created by rgerst on 26.10.18.
//


#pragma once
#include <misaxx/core/filesystem/misa_filesystem.h>
#include <misaxx/core/utils/dynamic_singleton_map.h>
#include <misaxx/core/utils/cache.h>

namespace misaxx {
    /**
     * Base class for all caches
     */
    struct misa_cache {

        using attachment_type = misaxx::utils::dynamic_singleton_map<misa_serializable>;
        using attachment_cache_type = misaxx::utils::memory_cache<attachment_type>;

        /**
         * Objects that are attached to this cache
         */
        attachment_cache_type attachments;

        /**
         * Links the cache to a filesystem location.
         * Assumes that data already exists within the location.
         * @param t_location
         */
        virtual void link(const boost::filesystem::path &t_location, const std::shared_ptr<misa_description_storage> &t_description) = 0;

        /**
         * Describes the contents of this cache using filesystem metadata
         * @return
         */
        virtual std::shared_ptr<misa_description_storage> describe() const = 0;

        /**
         * Returns the location of this cache in the filesystem
         * @return
         */
        virtual boost::filesystem::path get_location() const = 0;

        /**
         * Unique location of this cache, which should be not equal with get_location() if a cache contains sub-caches
         * @return
         */
        virtual boost::filesystem::path get_unique_location() const = 0;

        /**
         * Returns true if the cache has currently data
         * @return
         */
        virtual bool has_data() = 0;

        /**
         * Applies postprocessing (e.g. saving data) after analysis
         */
        virtual void postprocess() {

        }

        /**
         * Returns the location of this cache as internal filesystem path
         * @return
         */
        boost::filesystem::path get_location_in_filesystem() const;

        /**
         * Returns the unique location of this cache as internal filesystem path
         * @return
         */
        boost::filesystem::path get_unique_location_in_filesystem() const;

        /**
         * Returns the location interface of this cache
         * It should match the get_location() and get_unique_location() functions
         * Can be directly used for misa_locatable instances
         * Creates the location interface if necessary
         * @return
         */
        virtual std::shared_ptr<const misa_location> get_location_interface() const = 0;

    };
}
