//
// Created by rgerst on 28.11.18.
//

#pragma once

#include <misaxx/misa_cached_data.h>
#include <misaxx/caches/misa_image_file_cache.h>

namespace misaxx {
    template<class Image> struct misa_image_file : public misa_cached_data<misa_image_file_cache<Image>>{
        Image read_image() const {
            return this->access_readonly().get().clone();
        }

        void write_image(Image t_data) {
            this->access_write().set(std::move(t_data));
        }
    };
}



