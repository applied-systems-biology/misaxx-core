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
         * Links the cache to a filesystem location
         * @param t_location
         */
        virtual void link(const filesystem::const_entry &t_location) = 0;
    };
}
