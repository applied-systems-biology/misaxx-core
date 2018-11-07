//
// Created by rgerst on 02.11.18.
//


#pragma once

#include "misa_file_pattern.h"
#include "misa_file_stack_pattern.h"

namespace misaxx {
    /**
     * File pattern with predefined extensions that cover common image formats
     */
    struct misa_image_file_stack_pattern : public misa_file_stack_pattern {

        misa_image_file_stack_pattern() : misa_file_stack_pattern({ ".tif", ".tiff", ".png",
                                                                    ".bmp", ".pbm", ".pgm",
                                                                    ".ppm", ".jpeg", ".jpg",
                                                                    ".jpe", ".jp2" }) {

        }

        void from_json(const nlohmann::json &t_json) override {
            if(t_json.find("extensions") != t_json.end())
                misa_file_stack_pattern::from_json(t_json);
        }

        std::string get_serialization_id() const override {
            return "misa-image-file-stack-pattern";
        }
    };
}
