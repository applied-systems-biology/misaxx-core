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
         * Import the data from a filesystem location
         * @param t_entry
         */
        virtual void import_cache(const filesystem::const_entry &t_location) = 0;

        /**
         * Export the data to a filesystem location and set the metadata accordingly.
         * Please not that the data_type metadata is set by the cache itself.
         * @param t_location
         * @param t_metadata
         */
        virtual void export_cache(const filesystem::entry &t_location, const filesystem::misa_filesystem_metadata &t_metadata) = 0;

        /**
         * Clones the source cache into this one
         * Use this for 1:1 processing of data
         * @param t_location
         * @param t_source
         */
        virtual void clone_cache(const filesystem::entry &t_location, const misa_cache &t_source) = 0;
    };
}
