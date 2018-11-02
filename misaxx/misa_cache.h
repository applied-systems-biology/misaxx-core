//
// Created by rgerst on 26.10.18.
//


#pragma once
#include <misaxx/filesystem/misa_filesystem.h>

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
         * Gets a particular description from the description store
         * @tparam Description
         * @return
         */
        template<class Description> const Description &get_description() const {
            return describe()->get<Description>();
        }

        /**
         * Gets a particular description from the description store
         * @tparam Description
         * @return
         */
        template<class Description> Description &get_description() {
            return describe()->get<Description>();
        }

        /**
        * Gets a particular description from the description store
        * @tparam Description
        * @return
        */
        template<class Description> bool has_description() const {
            return describe()->has_description<Description>();
        }
    };
}
