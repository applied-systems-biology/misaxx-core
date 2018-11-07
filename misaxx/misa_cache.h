//
// Created by rgerst on 26.10.18.
//


#pragma once
#include <misaxx/filesystem/misa_filesystem.h>
#include <pattxx/runtime.h>

namespace misaxx {
    /**
     * Base class for all caches
     */
    struct misa_cache {

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
         * Returns true if the runtime is in simulation mode and no valid locations are passed.
         * @return
         */
        static bool is_simulating() {
            return pattxx::runtime::simulation_mode;
        }

    };
}
