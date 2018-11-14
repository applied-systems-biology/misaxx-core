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
#include <misaxx/misa_default_cache.h>

namespace misaxx {
    struct [[deprecated]] misa_unsafe_file : public misa_default_cache<cxxh::access::memory_cache<boost::filesystem::path>, misa_file_pattern, misa_file_description> {

        void do_link(const misa_file_description &t_description) override {
            this->set(get_location() / t_description.filename);
            this->set_unique_location(get());
        }

    protected:

        misa_file_description produce_description(const boost::filesystem::path &t_location, const misa_file_pattern &t_pattern) override {
            return t_pattern.produce(t_location);
        }

    };
}
