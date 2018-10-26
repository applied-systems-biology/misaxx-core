//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <coixx/opencv_image.h>
#include <cxxh/access/cache.h>
#include "misa_cache.h"
#include "misa_unsafe_image_file.h"

namespace misaxx {
    template<class Image> struct [[deprecated]] misa_unsafe_image_stack :
    public cxxh::access::cache<std::unordered_map<std::string, std::shared_ptr<misa_unsafe_image_file<Image>>>>, public misa_cache {

        using files_t = std::unordered_map<std::string, std::shared_ptr<misa_unsafe_image_file<Image>>>;

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-image-stack";

        files_t files;

        using image_type = Image;

        using cxxh::access::cache<files_t>::cache;

        bool supports_file(const boost::filesystem::path &t_path) const {
            return t_path.extension() == ".tif";
        }

        void link(const filesystem::const_entry &t_location) override {
            std::cout << "[MISA-cache] Linking cache of type " << DATA_TYPE << " to " << t_location->internal_path().string() << std::endl;
            for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_location->external_path()))) {
                if(supports_file(entry)) {
                    misa_unsafe_image_file<Image> img;
                    img.manual_link(entry.path().filename().string(), entry.path().extension().string(), entry.path());
                    files.insert({ entry.path().filename().string() , std::make_shared<misa_unsafe_image_file<Image>>(std::move(img)) });
                }
            }
        }

        void create(const filesystem::const_entry &t_location, const misa_filesystem_metadata &t_description) override {

        }

        misa_filesystem_metadata describe() override {
        }

        files_t &get() override {
            return files;
        }

        const files_t &get() const override {
            return files;
        }

        void set(files_t &&value) override {
            files = std::forward<files_t>(value);
        }

        bool has() const override {
            return true;
        }

        bool can_pull() const override {
            return true;
        }

        void pull() override {

        }

        void stash() override {

        }

        void push() override {

        }
    };
}
