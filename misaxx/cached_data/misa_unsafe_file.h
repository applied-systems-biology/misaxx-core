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
#include <misaxx/cached_data/patterns/misa_file_pattern.h>
#include "misa_cache.h"

namespace misaxx {
    struct [[deprecated]] misa_unsafe_file : public cxxh::access::memory_cache<boost::filesystem::path>, public misa_cache {

        static inline const std::string DATA_TYPE = "unsafe-file";

        std::shared_ptr<misa_filesystem_metadata> metadata;

        using cxxh::access::memory_cache<boost::filesystem::path>::memory_cache;

        /**
         * Links the file to any folder using the description
         * @param t_directory
         * @param t_description
         */
        void manual_link(const boost::filesystem::path &t_directory, const std::shared_ptr<misa_filesystem_metadata> &t_description) {
            metadata = t_description;
            auto &pattern = get_description<misa_file_pattern>();
            metadata->describe(pattern.produce(t_directory));
            this->set(t_directory / get_description<misa_file_description>().filename);
        }

        /**
         * Links to filesystem entry
         * @param t_location
         */
        void link(const filesystem::const_entry &t_location) override {
            manual_link(t_location->external_path(), t_location->metadata);
        }

        void create(const filesystem::entry &t_location, const std::shared_ptr<misa_filesystem_metadata> &t_description) override {
            // Update metadata in location
            t_location->metadata = t_description;

            link(t_location);
        }

        std::shared_ptr<misa_filesystem_metadata> describe() const override {
            return metadata;
        }
    };
}
