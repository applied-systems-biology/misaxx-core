//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_file.h"
#include <coixx/toolbox/toolbox_io.h>

namespace misaxx {
    template<class Image> struct misa_image_file : public misa_file {
        using image_type = Image;

        using misa_file::misa_file;

        /**
         * Loads the image described in this file.
         * If the loaded image does not have the right type, an exception will be thrown
         * @return
         */
        Image load() {
            return coixx::toolbox::load<Image>(path);
        }

        /**
        * Loads the image described in this file.
        * If necessary and supported, the image will be converted.
        * Otherwise an exception will be thrown.
        * @return
        */
        Image auto_load() {
            return coixx::toolbox::auto_load<Image>(path);
        }

        /**
         * Loads the image described in this file
         * @return
         */
        void save(const Image &t_img) {
            using namespace coixx;
            using namespace coixx::toolbox;
            t_img << save(path);
        }
    };
}
