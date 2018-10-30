//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <coixx/opencv_image.h>
#include <cxxh/access/cache.h>
#include <misaxx/cached_data/descriptions/misa_file_stack_description.h>
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

        misa_file_stack_description description;

        using image_type = Image;

        using cxxh::access::memory_cache<misa_unsafe_image_stack_contents<Image>>::cache;

        bool supports_file(const boost::filesystem::path &t_path) const {
            return t_path.extension() == ".tif";
        }

        void link(const filesystem::const_entry &t_location) override {
            description = t_location->metadata.get_description<misa_file_stack_description>();

            auto &files = this->get();

            if(description.has_files()) {
                const auto base_path = t_location->external_path();
                for(const auto &entry : description.files) {
                    misa_unsafe_image_file<Image> img;
                    const auto entry_path = base_path / entry.second.filename;
                    img.description.filename = entry_path.filename().string();
                    img.description.filetype = entry_path.extension().string();
                    img.set(entry_path);

                    files.insert({ entry.second.filename , misa_cached_data<misa_unsafe_image_file<Image>>(std::move(img)) });
                }
            }
            else {
                for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_location->external_path()))) {
                    if(supports_file(entry)) {
                        misa_unsafe_image_file<Image> img;
                        img.description.filename = entry.path().filename().string();
                        img.description.filetype = entry.path().extension().string();
                        img.set(entry.path());

                        files.insert({ entry.path().filename().string(), misa_cached_data<misa_unsafe_image_file<Image>>(std::move(img)) });

                        // Create description entry
                        misa_file_description entry_description;
                        entry_description.filename = entry.path().filename().string();
                        entry_description.filetype = entry.path().extension().string();
                        description.files.insert({ entry_description.filename, entry_description });
                    }
                }
            }
        }

        void create(const filesystem::const_entry &t_location, const misa_filesystem_metadata &t_description) override {
            description = t_description.get_description<misa_file_stack_description>();
        }

        misa_filesystem_metadata describe() override {
            misa_filesystem_metadata result;
            result.describe(description);
            return result;
        }
    };
}
