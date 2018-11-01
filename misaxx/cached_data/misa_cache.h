//
// Created by rgerst on 26.10.18.
//


#pragma once
#include "../filesystem/misa_filesystem.h"

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
        virtual void link(const filesystem::const_entry &t_location) = 0;

        /**
         * Links the cache to a filesystem location.
         * Data is created based on the parameters.
         * @param t_location
         * @param t_description Description of contents
         */
        virtual void create(const filesystem::entry &t_location, const std::shared_ptr<misa_filesystem_metadata> &t_description) = 0;

        /**
         * Describes the contents of this cache using filesystem metadata
         * @return
         */
        virtual std::shared_ptr<misa_filesystem_metadata> describe() const = 0;

        /**
         * Gets a particular description from the description store
         * @tparam Description
         * @return
         */
        template<class Description> const Description &get_description() const {
            return describe()->get_description<Description>();
        }

        /**
         * Gets a particular description from the description store
         * @tparam Description
         * @return
         */
        template<class Description> Description &get_description() {
            return describe()->get_description<Description>();
        }
    };
}
