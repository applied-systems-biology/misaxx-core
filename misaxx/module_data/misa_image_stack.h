//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_file_stack.h"
#include "misa_image_file.h"
#include <coixx/opencv_image.h>

namespace misaxx {
    template<class Image> struct misa_image_stack : public misa_file_stack<misa_image_file<Image>> {

        using image_type = Image;

        using misa_file_stack<misa_image_file<Image>>::misa_file_stack;

        bool supports_file(const boost::filesystem::path &t_path) const override {
            return t_path.extension() == ".tif";
        }

        std::string dataString() override {
            return "image_stack";
        }
    };
}
