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

    template<class Image> struct [[deprecated]] misa_unsafe_image_stack : public misa_default_cache<misa_unsafe_image_stack_contents<Image>,
            cxxh::access::memory_cache<misa_unsafe_image_stack_contents<Image>>> {

        std::shared_ptr<misa_description_storage> metadata;

        using image_type = Image;

        void simulate_link() override {
            metadata->access<misa_file_stack_pattern>();
            metadata->access<misa_file_stack_description>();
        }

        void do_link() override {
            if(!this->describe()->has_description()) {
                metadata->set(this->describe()->template get<misa_image_file_stack_pattern>().produce(this->get_location()));
            }

            auto &files = this->get();
            for(const auto &kv : this->describe()->template get<misa_file_stack_description>().files) {
                misa_cached_data<misa_unsafe_image_file<Image>> cache;
                cache.suggest_link(this->get_location(), misa_description_storage::with(kv.second)); // We link manually with the loaded description
                files.insert({ kv.first, cache });
            }

            set_unique_location(this->get_location());
        }
    };
}
