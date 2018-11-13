//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include <misaxx/misa_module_declaration_base.h>
#include <misaxx/misa_cached_data.h>
#include <misaxx/filesystem/misa_filesystem_entry.h>
#include <cxxh/access/cache.h>
#include <boost/algorithm/string.hpp>
#include <misaxx/descriptions/misa_file_description.h>
#include <cxxh/access/memory_cache.h>
#include <misaxx/patterns/misa_file_pattern.h>
#include <misaxx/misa_cache.h>
#include <misaxx/runtime/misa_runtime_base.h>

namespace misaxx {
    struct [[deprecated]] misa_unsafe_file : public cxxh::access::memory_cache<boost::filesystem::path>, public misa_cache {

        static inline const std::string DATA_TYPE = "unsafe-file";

        std::shared_ptr<misa_description_storage> metadata;

        using cxxh::access::memory_cache<boost::filesystem::path>::memory_cache;

        /**
         * Links the file to any folder using the description
         * @param t_directory
         * @param t_description
         */
        void link(const boost::filesystem::path &t_directory, const std::shared_ptr<misa_description_storage> &t_description) override {
            metadata = t_description;
            m_location = t_directory;

            // If we simulate, just announce the existence of pattern & description
            if(misa_runtime_base::instance().is_simulating()) {
                metadata->access<misa_file_pattern>();
                metadata->access<misa_file_description>();
                return;
            }

            if(!describe()->has_description()) {
                metadata->set(describe()->get<misa_file_pattern>().produce(t_directory));
            }
            this->set(t_directory / describe()->get<misa_file_description>().filename);
        }

        std::shared_ptr<misa_description_storage> describe() const override {
            return metadata;
        }

        boost::filesystem::path get_location() const override {
            return m_location;
        }

    private:
        boost::filesystem::path m_location;
    };
}
