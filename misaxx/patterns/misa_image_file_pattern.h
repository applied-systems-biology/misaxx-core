//
// Created by rgerst on 02.11.18.
//


#pragma once

#include <misaxx/patterns/misa_file_pattern.h>

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

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return create_serialization_id_hierarchy(*this, { misa_file_pattern::get_serialization_id_hierarchy() });
        }
    };

    inline void to_json(nlohmann::json& j, const misa_image_file_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_image_file_pattern& p) {
        p.from_json(j);
    }
}
