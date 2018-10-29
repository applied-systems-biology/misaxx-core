//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/misa_module_declaration_base.h"
#include "misa_cached_data.h"
#include "misaxx/filesystem/misa_filesystem_entry.h"
#include <cxxh/access/cache.h>
#include <boost/algorithm/string.hpp>
#include <misaxx/cached_data/descriptions/misa_file_description.h>
#include "misa_cache.h"

namespace misaxx {
    struct [[deprecated]] misa_unsafe_file : public cxxh::access::cache<boost::filesystem::path> , public misa_cache {

        static inline const std::string DATA_TYPE = "unsafe-file";

        /**
         * Complete filename
         */
        std::string filename;

        /**
         * Filetype (extension) with dot
         */
        std::string filetype;

        /**
         * Full path
         */
        boost::filesystem::path path;

        using cxxh::access::cache<boost::filesystem::path>::cache;

        /**
         * Links from code
         * @param t_filename
         * @param t_filetype
         * @param t_path
         */
        void manual_link(std::string t_filename, std::string t_filetype, boost::filesystem::path t_path) {
            std::cout << "[MISA-cache] Manually linking cache of type " << DATA_TYPE << std::endl;

            filename = std::move(t_filename);
            filetype = std::move(t_filetype);
            path = std::move(t_path);
        }

        /**
         * Links to filesystem entry
         * @param t_location
         */
        void link(const filesystem::const_entry &t_location) override {
            std::cout << "[MISA-cache] Linking cache of type " << DATA_TYPE << " to " << t_location->internal_path().string() << std::endl;

            const auto &description = t_location->metadata.get_description<misa_file_description>();
            if(description.has_filename()) {
                filename = description.filename;
            }
            else {
                if(description.has_filetype()) {
                    throw std::runtime_error("Cannot automatically determine filename without filetype!");
                }

                // Try to automatically find a file
                for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_location->external_path()))) {
                    if(boost::filesystem::is_regular_file(entry) && boost::iequals(entry.path().extension().string(), filetype)) {
                        filename = entry.path().filename().string();
                        break;
                    }
                }
            }

            if(filename.empty()) {
                throw std::runtime_error("Could not find a file in the storage!");
            }

            path = t_location->external_path() / filename;
        }

        void create(const filesystem::const_entry &t_location, const misa_filesystem_metadata &t_description) override {

        }

        misa_filesystem_metadata describe() override {
        }

        boost::filesystem::path &get() override {
            return path;
        }

        const boost::filesystem::path &get() const override {
            return path;
        }

        void set(boost::filesystem::path &&value) override {
            path = std::forward<boost::filesystem::path>(value);
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
