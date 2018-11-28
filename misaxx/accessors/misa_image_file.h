//
// Created by rgerst on 28.11.18.
//

#pragma once

#include <misaxx/misa_cached_data.h>
#include <misaxx/caches/misa_image_file_cache.h>

namespace misaxx {
    template<class Image> struct misa_image_file : public misa_cached_data<misa_image_file_cache<Image>>{
        /**
         * Clones the image content read from access_readonly()
         * @return
         */
        Image clone() const {
            return this->access_readonly().get().clone();
        }

        /**
         * Writes image data into the current file
         * @param t_data
         */
        void write(Image t_data) {
            this->access_write().set(std::move(t_data));
        }
    };
}



