//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <misaxx/caches/misa_unsafe_file.h>
#include <coixx/toolbox/toolbox_io.h>

namespace misaxx {
    /**
     * Legacy image file that does not use the properties of the cache system
     * Please do not use it
     * @tparam Image
     */
    template<class Image> struct [[deprecated]] misa_unsafe_image_file : public misa_unsafe_file {

        using image_type = Image;

        using misa_unsafe_file::misa_unsafe_file;

        /**
         * Loads the image described in this file.
         * If the loaded image does not have the right type, an exception will be thrown
         * @return
         */
        Image load() {
            return coixx::toolbox::load<Image>(get());
        }

        /**
        * Loads the image described in this file.
        * If necessary and supported, the image will be converted.
        * Otherwise an exception will be thrown.
        * @return
        */
        Image auto_load() {
            return coixx::toolbox::auto_load<Image>(get());
        }

        /**
         * Loads the image described in this file
         * @return
         */
        void save(const Image &t_img) {
            using namespace coixx;
            using namespace coixx::toolbox;
            t_img << coixx::toolbox::save(get());
        }
    };
}
