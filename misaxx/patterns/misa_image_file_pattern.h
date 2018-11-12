//
// Created by rgerst on 02.11.18.
//


#pragma once

#include "misa_file_pattern.h"

namespace misaxx {
    struct misa_image_file_pattern : public misa_file_pattern {
        misa_image_file_pattern() : misa_file_pattern({ ".tif", ".tiff", ".png",
                                                        ".bmp", ".pbm", ".pgm",
                                                        ".ppm", ".jpeg", ".jpg",
                                                        ".jpe", ".jp2" }) {

        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "patterns/image-file");
        }

    };
}
