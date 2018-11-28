//
// Created by rgerst on 28.11.18.
//

#pragma once

#include <misaxx/caches/misa_exported_attachments_cache.h>
#include <misaxx/misa_cached_data.h>
#include <opencv2/opencv.hpp>
#include <misaxx/misa_default_description_accessors.h>

namespace misaxx {
    /**
     * Allows exporting attachments to a specified file
     */
    struct misa_exported_attachments : public misa_cached_data<misa_exported_attachments_cache>,
            public misa_description_accessors_from_cache<misa_exported_attachments_cache, misa_exported_attachments> {
    };
}




