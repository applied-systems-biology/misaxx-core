//
// Created by rgerst on 28.11.18.
//

#pragma once

#include <misaxx/core/caches/misa_exported_attachments_cache.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/misa_default_description_accessors.h>

namespace misaxx {
    /**
     * This cache exports all attached data into a separate file.
     */
    struct misa_exported_attachments : public misa_cached_data<misa_exported_attachments_cache>,
            public misa_description_accessors_from_cache<misa_exported_attachments_cache, misa_exported_attachments> {

        using misa_cached_data<misa_exported_attachments_cache>::suggest_export_location;

        /**
         * Suggests a filename for the exported attachments
         * @param t_filesystem
         * @param t_path
         */
        void suggest_export_location(const misa_filesystem &t_filesystem,
                                     const boost::filesystem::path &t_path);

    };
}




