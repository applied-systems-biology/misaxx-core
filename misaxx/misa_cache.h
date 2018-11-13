//
// Created by rgerst on 26.10.18.
//


#pragma once
#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/runtime/misa_runtime_base.h>
#include <cxxh/containers/dynamic_singleton_map.h>
#include <cxxh/access/memory_cache.h>

namespace misaxx {
    /**
     * Base class for all caches
     */
    struct misa_cache {

        using attachment_type = cxxh::containers::dynamic_singleton_map<misa_serializeable>;
        using attachment_cache_type = cxxh::access::memory_cache<attachment_type>;

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
         * Returns true if the runtime is in simulation mode and no valid locations are passed.
         * @return
         */
        static bool is_simulating() {
            return misaxx::misa_runtime_base::instance().is_simulating();
        }

    };
}
