//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/misa_module_declaration_base.h"
#include "misa_cached_data.h"
#include "misaxx/filesystem/misa_filesystem_entry.h"
#include <coipxx/cache.h>
#include "misa_cache.h"

namespace misaxx {
    struct misa_unsafe_file : public coipxx::cache<boost::filesystem::path> , public misa_cache {

        /**
         * Used by the misa_cache_registry
         */
        static inline const std::string DATA_TYPE = "unsafe-file";

        std::string name;
        boost::filesystem::path path;

        using coipxx::cache<boost::filesystem::path>::cache;

        void import_from_filesystem(const misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            if(!t_module.m_runtime->is_building_schema()) {
                const auto &vfs = t_module.filesystem.imported->access(t_path);
//                vfs->data_string = dataString();
                path = vfs->external_path();
            }
        }

        void export_to_filesystem(misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            if(!t_module.m_runtime->is_building_schema()) {
                const auto &vfs = t_module.filesystem.exported->access(t_path);
//                vfs->data_string = dataString();
                path = vfs->external_path();
            }
        }
    };
}
