//
// Created by rgerst on 28.11.18.
//

#pragma once

#include <misaxx/caches/misa_exported_attachments_cache.h>
#include <misaxx/misa_cached_data.h>
#include <opencv2/opencv.hpp>

namespace misaxx {
    /**
     * Allows exporting attachments to a specified file
     */
    struct misa_exported_attachments : public misa_cached_data<misa_exported_attachments_cache> {
    };
}




