//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <coixx/image.h>
#include <cxxh/cache.h>
#include <misaxx/descriptions/misa_file_stack_description.h>
#include <misaxx/patterns/misa_file_stack_pattern.h>
#include <misaxx/patterns/misa_image_file_stack_pattern.h>
#include <misaxx/misa_cache.h>
#include <misaxx/misa_cached_data.h>
#include <misaxx/caches/misa_image_file.h>

namespace misaxx {

    /**
     * Stack of images stored in a misa_image_stack
     */
    template<class Image> using misa_image_stack_t = std::unordered_map<std::string, misa_cached_data<misa_image_file<Image>>>;

    /**
     * Simple stack of images of cv::Mat or any of coixx::image<T>
     * @tparam Image
     */
    template<class Image> struct misa_image_stack : public misa_default_cache<cxxh::memory_cache<misa_image_stack_t<Image>>, misa_image_file_stack_pattern, misa_file_stack_description> {

        using image_type = Image;

        void do_link(const misa_file_stack_description &t_description) override {
            auto &files = this->get();
            for(const auto &kv : t_description.files) {
                misa_cached_data<misa_image_file<Image>> cache;
                cache.suggest_link(this->get_location(), misa_description_storage::with(kv.second)); // We link manually with the loaded description
                files.insert({ kv.first, cache });
            }

            this->set_unique_location(this->get_location());
        }

    protected:

        misa_file_stack_description produce_description(const boost::filesystem::path &t_location, const misa_image_file_stack_pattern &t_pattern) override {
            return t_pattern.produce(t_location);
        }

    public:

        /**
         * Convenience function that returns the image stack contents
         * @param t_cache
         * @return
         */
        static misa_image_stack_t<Image> get_images(const misa_cached_data<misa_image_stack<Image>> &t_cache) {
            return t_cache.access_readonly().get();
        }

        /**
         * Convenience function that returns true if the image stack is empty
         * @param t_cache
         * @return
         */
        static bool is_empty(const misa_cached_data<misa_image_stack<Image>> &t_cache) {
            return t_cache.access_readonly().get().empty();
        }

    };
}
