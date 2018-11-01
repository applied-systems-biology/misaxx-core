//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <coixx/opencv_image.h>
#include <cxxh/access/cache.h>
#include <misaxx/cached_data/descriptions/misa_file_stack_description.h>
#include <misaxx/cached_data/patterns/misa_file_stack_pattern.h>
#include "misa_cache.h"
#include "misa_unsafe_image_file.h"

namespace misaxx {

    template<class Image> using misa_unsafe_image_stack_contents = std::unordered_map<std::string, misa_cached_data<misa_unsafe_image_file<Image>>>;

    template<class Image> struct [[deprecated]] misa_unsafe_image_stack :
    public cxxh::access::memory_cache<misa_unsafe_image_stack_contents<Image>>, public misa_cache {

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-image-stack";

        std::shared_ptr<misa_filesystem_metadata> metadata;

        using image_type = Image;

        using cxxh::access::memory_cache<misa_unsafe_image_stack_contents<Image>>::cache;

        void link(const filesystem::const_entry &t_location, const std::shared_ptr<misa_filesystem_metadata> &t_alternative_description) override {
            metadata = t_location->metadata;
            auto &pattern = get_description<misa_file_stack_pattern>();
            metadata->describe(pattern.produce(t_location->external_path()));

            auto &files = this->get();
            for(const auto &kv : get_description<misa_file_stack_description>().files) {
                misa_cached_data<misa_unsafe_image_file<Image>> cache;
                cache.suggest_link() // We temporary reinterpret the data here
                cache.cache->set(t_location->external_path() / kv.second.filename);
                files.insert({ kv.first, cache });
            }
        }

        void create(const filesystem::entry &t_location, const std::shared_ptr<misa_filesystem_metadata> &t_description) override {
            auto &files = this->get();
            for(const auto &kv : get_description<misa_file_stack_description>().files) {
                misa_cached_data<misa_unsafe_image_file<Image>> cache;
                cache.suggest
                cache.cache->set(t_location->external_path() / kv.second.filename);
                files.insert({ kv.first, cache });
            }
        }

        std::shared_ptr<misa_filesystem_metadata> describe() const override {
            return metadata;
        }
    };
}
