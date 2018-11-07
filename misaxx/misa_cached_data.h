//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <cxxh/containers/dynamic_singleton_map.h>
#include <cxxh/access/readonly_access.h>
#include <cxxh/access/readwrite_access.h>
#include <cxxh/access/write_access.h>
#include <cxxh/access/memory_cache.h>
#include <misaxx/misa_module_declaration_base.h>
#include <misaxx/misa_serializeable.h>
#include <misaxx/misa_cache.h>

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
        using attachment_type = cxxh::containers::dynamic_singleton_map<misa_serializeable>;
        using attachment_cache_type = cxxh::access::memory_cache<attachment_type>;

        /**
         * Main cache for the data
         */
        std::shared_ptr<Cache> cache;

        /**
         * Attachments to this cache. Can be used by algorithms to communicate results.
         */
        std::shared_ptr<attachment_cache_type> attachments;

        misa_cached_data() {
            attachments = std::make_shared<attachment_cache_type>();
        }

        explicit misa_cached_data(Cache t_cache) : cache(std::make_shared<Cache>(std::move(t_cache))) {
        }

        /**
         * Returns true if this cache is set from a parent module
         * @return
         */
        bool is_externally_set() const {
            return cache && !cache.unique();
        }

        /**
         * Links this cache to a filesystem location if not already set.
         * This calls the internal linkage method of the cached data.
         * If linking with filesystem entries, you can use the other methods
         * @param t_location
         * @param t_description
         */
        void suggest_link(const boost::filesystem::path &t_location, const std::shared_ptr<misa_description_storage> &t_description) {
            if(!cache) {
                cache = std::make_shared<Cache>();
                std::cout << "[Cache] Manually linking " << t_location << " into cache of type " << Cache::DATA_TYPE << std::endl;
                cache->link(t_location, t_description);
            }
        }

        /**
         * Links this cache to a filesystem location if not already set.
         * The data is assumed to already exist. Necessary metadata should be contained within the filesystem.
         * Metadata is not copied during this operation.
         * @param t_location
         */
        void suggest_link(const filesystem::const_entry &t_location) {
            if(!cache) {
                cache = std::make_shared<Cache>();
                std::cout << "[Cache] Linking " << t_location->internal_path() << " [" << t_location->external_path() << "] into cache of type " << Cache::DATA_TYPE << std::endl;
                cache->link(t_location->external_path(), t_location->metadata);
            }
        }

        /**
        * Links this cache to a filesystem location if not already set.
        * The data is assumed to not exist. Necessary metadata must be obtained from linked caches or manually set in code.
        * Metadata is copied if it is not unique
        * @param t_location
        */
        void suggest_create(const filesystem::entry &t_location, const std::shared_ptr<misa_description_storage> &t_description) {
            if(!cache) {
                cache = std::make_shared<Cache>();
                std::cout << "[Cache] Creating " << t_location->internal_path() << " [" << t_location->external_path() << "] as cache of type " << Cache::DATA_TYPE << std::endl;

                // Create the directory if necessary
                if(!boost::filesystem::exists(t_location->external_path())) {
                    std::cout << "[Cache] Creating directory "  << t_location->external_path() << std::endl;
                    boost::filesystem::create_directories(t_location->external_path());
                }

                // Metadata is copied into the export location
                if(t_description.unique()) {
                    t_location->metadata = t_description;
                }
                else {
                    t_location->metadata = std::make_shared<misa_description_storage>(*t_description);
                }

                cache->link(t_location->external_path(), t_location->metadata);
            }
        }

        /**
         * Links this cache to an imported filesystem path.
         * This path must exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         */
        void suggest_import_location(const misa_filesystem &t_filesystem, const boost::filesystem::path &t_path) {
            if(!cache) {
                suggest_link(t_filesystem.imported->at(t_path));
            }
        }

        /**
         * Links this cache to an exported filesystem path.
         * This path must not exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         * @param t_description
         */
        void suggest_export_location(const misa_filesystem &t_filesystem,
                const boost::filesystem::path &t_path, const std::shared_ptr<misa_description_storage> &t_description) {
            if(!cache) {
                if(t_filesystem.exported->has_subpath(t_path))
                    throw std::runtime_error("Suggested export location exported/" + t_path.string() + " is already used!");
                suggest_create(t_filesystem.exported->access(t_path), t_description);
            }
        }

        /**
         * Returns a description of the current cache
         * @return
         */
        std::shared_ptr<misa_description_storage> describe() {
            return cache->describe();
        }

        /**
         * Thread-safe read-only access to the data.
         * @return
         */
        readonly_access <value_type > access_readonly() const {
            return readonly_access<value_type >(*cache);
        }

        /**
        * Thread-safe read & write access to the data.
        * Other threads are blocked from access.
        * @return
        */
        readwrite_access <value_type > access_readwrite() {
            return readwrite_access<value_type >(*cache);
        }

        /**
         * Thread-safe write-only access to the data.
         * Other threads are blocked from access.
         * @return
         */
        write_access <value_type > access_write() {
            return write_access<value_type >(*cache);
        }

        /**
         * Attaches data
         * @tparam Attachment
         * @param attachment
         */
        template<class Attachment> void attach(Attachment attachment) {
            readwrite_access <attachment_type > access(*attachments);
            access.get().insert(std::move(attachment));
        }

        /**
         * Gets copy of attached data.
         * Throws exception if attachment does not exist
         * @tparam Attachment
         * @return
         */
        template<class Attachment> Attachment get_attachment() const {
            readwrite_access <attachment_type > access(*attachments);
            return access.get().at<Attachment>();
        }

        /**
        * Gets copy of attached data.
        * Throws exception if attachment does not exist
        * @tparam Attachment
        * @return
        */
        template<class Attachment> bool has_attachment() const {
            readonly_access <attachment_type > access(*attachments);
            return access.get().has<Attachment>();
        }
    };
}
