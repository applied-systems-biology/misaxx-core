//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <coixx/opencv_image.h>
#include <cxxh/access/cache.h>
#include "misa_cache.h"
#include "misa_unsafe_image_file.h"

namespace misaxx {
    template<class Image> struct misa_unsafe_image_stack : public cxxh::access::cache<std::unordered_map<std::string, misa_unsafe_image_file<Image>>>, public misa_cache {

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-image-stack";

        using image_type = Image;

        using cxxh::access::cache<std::unordered_map<std::string, misa_unsafe_image_file<Image>>>::cache;

        bool supports_file(const boost::filesystem::path &t_path) const override {
            return t_path.extension() == ".tif";
        }

        std::string dataString() override {
            return "image_stack";
        }
    };
}
