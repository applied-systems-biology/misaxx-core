//
// Created by rgerst on 28.11.18.
//

#pragma once

#include <misaxx/caches/misa_image_stack_cache.h>

namespace misaxx {

    /**
     * A simple stack of images
     * @tparam Image cv::Mat or any of coixx::image<C>
     */
    template<class Image>
    class misa_image_stack : public misa_cached_data<misa_image_stack_cache<Image>>,
                             public misa_description_accessors_from_cache<misa_image_stack_cache<Image>, misa_image_stack<Image>> {

        using iterator = typename misa_image_stack_t<Image>::iterator;
        using const_iterator = typename misa_image_stack_t<Image>::const_iterator;

        iterator begin() {
            return this->data->get().begin();
        }

        iterator end() {
            return this->data->get().end();
        }

        const_iterator begin() const {
            return this->data->get().begin();
        }

        const_iterator end() const {
            return this->data->get().end();
        }

        const_iterator find(const std::string &t_name) const {
            return this->data->get().find(t_name);
        }

        misa_image_file<Image> at(const std::string &t_name) const {
            return this->data->get().at(t_name);
        }

        /**
         * Returns true if there are no images in this stack
         * @return
         */
        bool empty() {
            return this->access_readonly().get().empty();
        }
    };
}




