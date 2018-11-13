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
    struct [[deprecated]] misa_unsafe_file : public misa_default_cache<cxxh::access::memory_cache<boost::filesystem::path>> {

        void simulate_link() override {
            describe()->access<misa_file_pattern>();
            describe()->access<misa_file_description>();
        }

        void do_link() override {
            if(!describe()->has_description()) {
                describe()->set(describe()->get<misa_file_pattern>().produce(get_location()));
            }
            this->set(get_location() / describe()->get<misa_file_description>().filename);
            this->set_unique_location(get());
        }
    };
}
