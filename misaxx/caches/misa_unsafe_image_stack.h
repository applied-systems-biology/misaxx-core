//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <coixx/opencv_image.h>
#include <cxxh/access/cache.h>
#include <misaxx/descriptions/misa_file_stack_description.h>
#include <misaxx/patterns/misa_file_stack_pattern.h>
#include <misaxx/patterns/misa_image_file_stack_pattern.h>
#include <misaxx/misa_cache.h>
#include "misa_unsafe_image_file.h"

namespace misaxx {

    template<class Image> using misa_unsafe_image_stack_contents = std::unordered_map<std::string, misa_cached_data<misa_unsafe_image_file<Image>>>;

    template<class Image> struct [[deprecated]] misa_unsafe_image_stack :
    public cxxh::access::memory_cache<misa_unsafe_image_stack_contents<Image>>, public misa_cache {

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-image-stack";

        std::shared_ptr<misa_description_storage> metadata;

        using image_type = Image;

        using cxxh::access::memory_cache<misa_unsafe_image_stack_contents<Image>>::cache;

        void link(const boost::filesystem::path &t_location, const std::shared_ptr<misa_description_storage> &t_description) override {
            metadata = t_description;

            if(is_simulating()) {
                metadata->access<misa_file_stack_pattern>();
                metadata->access<misa_file_stack_description>();
                return;
            }

            if(!t_description->has_description()) {
                metadata->set(t_description->get<misa_image_file_stack_pattern>().produce(t_location));
            }

            auto &files = this->get();
            for(const auto &kv : t_description->get<misa_file_stack_description>().files) {
                misa_cached_data<misa_unsafe_image_file<Image>> cache;
                cache.suggest_link(t_location, misa_description_storage::with(kv.second)); // We link manually with the loaded description
                files.insert({ kv.first, cache });
            }
        }

        std::shared_ptr<misa_description_storage> describe() const override {
            return metadata;
        }
    };
}
