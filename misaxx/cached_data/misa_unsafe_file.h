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
#include <cxxh/access/memory_cache.h>
#include "misa_cache.h"

namespace misaxx {
    struct [[deprecated]] misa_unsafe_file : public cxxh::access::memory_cache<boost::filesystem::path> , public misa_cache {

        static inline const std::string DATA_TYPE = "unsafe-file";


        misa_file_description description;

        using cxxh::access::memory_cache<boost::filesystem::path>::memory_cache;

        /**
         * Links to filesystem entry
         * @param t_location
         */
        void link(const filesystem::const_entry &t_location) override {
            std::cout << "[MISA-cache] Linking cache of type " << DATA_TYPE << " to " << t_location->internal_path().string() << std::endl;

            description = t_location->metadata.get_description<misa_file_description>();
            if (description.has_filetype() && !description.has_filename()) {
                // Try to automatically find a file
                for (const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_location->external_path()))) {
                    if (boost::filesystem::is_regular_file(entry) && boost::iequals(entry.path().extension().string(), description.filetype)) {
                        description.filename = entry.path().filename().string();
                        break;
                    }
                }
            }

            if(!description.has_filename()) {
                throw std::runtime_error("Could not find a file in the storage!");
            }

            this->set(t_location->external_path() / description.filename);
        }

        void create(const filesystem::const_entry &t_location, const misa_filesystem_metadata &t_description) override {

            std::cout << "[MISA-cache] Creating cache of type " << DATA_TYPE << " at internal location " << t_location->internal_path().string() << std::endl;

            description = t_description.get_description<misa_file_description>();

            if(!description.has_filename()) {
                if(!description.has_filetype())
                    throw std::runtime_error("Cannot create new file without filename and filetype!");

                description.filename = "file" + description.filetype;
            }

            this->set(t_location->external_path() / description.filename);
        }

        misa_filesystem_metadata describe() override {
            misa_filesystem_metadata result;
            result.describe(description);
            return result;
        }
    };
}
