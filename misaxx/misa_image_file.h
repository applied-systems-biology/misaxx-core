//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_file.h"

namespace misaxx {
    template<class Image> struct misa_image_file : public misa_file {
        using image_type = Image;
    };
}
