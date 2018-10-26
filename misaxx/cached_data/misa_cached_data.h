//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <cxxh/containers/dynamic_singleton_map.h>
#include <cxxh/access/readonly_access.h>
#include <cxxh/access/readwrite_access.h>
#include <cxxh/access/write_access.h>
#include "misaxx/misa_module_declaration_base.h"
#include "../ome_data/misa_metadata.h"
#include "misa_cache.h"

namespace misaxx {

    template<typename Value> using readonly_access = cxxh::access::readonly_access<Value>;
    template<typename Value> using readwrite_access = cxxh::access::readwrite_access<Value>;
    template<typename Value> using write_access = cxxh::access::write_access<Value>;

    /**
     * Shared pointer that contains a data cache and additional functions to import/export data
     * @tparam Cache
     */
    template<class Cache> struct misa_cached_data {

        using cache_type = Cache;
        using value_type = typename Cache::value_type;

        std::shared_ptr<Cache> cache;

        /**
         * Returns true if this cache is set from a parent module
         * @return
         */
        bool is_externally_set() const {
            return cache && !cache.unique();
        }

        /**
         * Links this cache to a filesystem location if not already set
         * @param t_location
         */
        void suggest_link(const filesystem::const_entry &t_location) {
            if(!cache) {
                cache = std::make_shared<Cache>();
                cache->link(t_location);
            }
        }

        /**
         * Links this cache to an imported filesystem path.
         * This path must exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         */
        void suggest_import_location(const filesystem::misa_filesystem &t_filesystem, const boost::filesystem::path &t_path) {
            if(!cache) {
                suggest_link(t_filesystem.imported->at(t_path));
            }
        }

        /**
         * Links this cache to an exported filesystem path.
         * This path must not exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         * @param t_metadata
         */
        void suggest_export_location(const filesystem::misa_filesystem &t_filesystem, const boost::filesystem::path &t_path, const filesystem::misa_filesystem_metadata &t_metadata) {
            if(!cache) {
                if(t_filesystem.exported->has_subpath(t_path))
                    throw std::runtime_error("Suggested export location exported/" + t_path.string() + " is already used!");
                suggest_link(t_filesystem.exported->access(t_path));
            }
        }

        readonly_access <value_type > access_readonly() {
            return readonly_access<value_type >(*cache);
        }

        readwrite_access <value_type > access_readwrite() {
            return readwrite_access<value_type >(*cache);
        }

        write_access <value_type > access_write() {
            return write_access<value_type >(*cache);
        }
    };
}
