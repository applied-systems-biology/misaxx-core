//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <cxxh/dynamic_singleton_map.h>
#include <cxxh/cache.h>
#include <misaxx/misa_module_declaration_base.h>
#include <misaxx/misa_serializeable.h>
#include <misaxx/misa_cache.h>
#include <misaxx/misa_cached_data_base.h>

namespace misaxx {

    /**
     * A shared pointer that manages cached data and comes with methods to import and export data from filesystem.
     * Please prefer interacting with cached data using this interface, as it automatically registers caches
     * into the runtime using misa_runtime_base::instance().register_cache.
     * @tparam Cache
     */
    template<class Cache>
    struct misa_cached_data : public misa_cached_data_base {

        using cache_type = Cache;
        using value_type = typename Cache::value_type;

        /**
         * Pointer to the internal cache data
         */
        std::shared_ptr<Cache> data;

        misa_cached_data() = default;

        /**
         * Creates cached data from an existing cache instance.
         * @param t_cache
         */
        explicit misa_cached_data(Cache t_cache) : data(std::make_shared<Cache>(std::move(t_cache))) {
        }

        /**
         * Returns true if this cache is set from a parent module
         * @return
         */
        bool is_externally_set() const override {
            return data && !data.unique();
        }

        /**
         * Returns true if this cached data contains data
         * @return
         */
        bool has_data() const override {
            return static_cast<bool>(data);
        }

        /**
         * Returns the cached data as pointer to misa_cache
         * @return
         */
        std::shared_ptr<misa_cache> get_cache_base() const override {
            return data;
        }

        /**
        * Links this cache to a filesystem location.
        * This calls the internal linkage method of the cached data.
        * If linking with filesystem entries, you can use the other methods
        * @param t_location
        * @param t_description
        */
        void force_link(const boost::filesystem::path &t_location,
                        const std::shared_ptr<misa_description_storage> &t_description) {
            if (!data)
                data = std::make_shared<Cache>();
            misa_runtime_base::instance().register_cache(data);
            std::cout << "[Cache] Linking " << t_location << " into cache of type " << typeid(Cache).name()
                      << std::endl;
            data->link(t_location, t_description);
        }

        /**
         * Links this cache to a filesystem location.
         * The data is assumed to already exist. Necessary metadata should be contained within the filesystem.
         * Metadata is not copied during this operation.
         * @param t_location
         */
        void force_link(const filesystem::const_entry &t_location) {
            if (!data)
                data = std::make_shared<Cache>();
            misa_runtime_base::instance().register_cache(data);

            // Special case simulation mode
            if (misaxx::misa_runtime_base::instance().is_simulating()) {
                std::cout << "[Cache] Linking " << t_location->internal_path() << " into cache of type "
                          << typeid(Cache).name() << std::endl;
                data->link("", t_location->metadata);
                return;
            }

            std::cout << "[Cache] Linking " << t_location->internal_path() << " [" << t_location->external_path()
                      << "] into cache of type " << typeid(Cache).name() << std::endl;
            data->link(t_location->external_path(), t_location->metadata);
        }

        /**
         * Links this cache to a filesystem location if not already set.
         * This calls the internal linkage method of the cached data.
         * If linking with filesystem entries, you can use the other methods
         * @param t_location
         * @param t_description
         */
        void suggest_link(const boost::filesystem::path &t_location,
                          const std::shared_ptr<misa_description_storage> &t_description) {
            if (!data) {
                data = std::make_shared<Cache>();
                force_link(t_location, t_description);
            }
        }

        /**
         * Links this cache to a filesystem location if not already set.
         * The data is assumed to already exist. Necessary metadata should be contained within the filesystem.
         * Metadata is not copied during this operation.
         * @param t_location
         */
        void suggest_link(const filesystem::const_entry &t_location) {
            if (!data) {
                data = std::make_shared<Cache>();
                force_link(t_location);
            }
        }

        /**
        * Links this cache to a filesystem location if not already set.
        * The data is assumed to not exist. Necessary metadata must be obtained from linked caches or manually set in code.
        * Metadata is copied if it is not unique
        * @param t_location
        */
        void suggest_create(const filesystem::entry &t_location,
                            const std::shared_ptr<misa_description_storage> &t_description) {
            if (!data) {
                data = std::make_shared<Cache>();
                misa_runtime_base::instance().register_cache(data);

                if (misaxx::misa_runtime_base::instance().is_simulating()) {
                    std::cout << "[Cache] Creating " << t_location->internal_path() << " as cache of type "
                              << typeid(Cache).name() << std::endl;
                    // Metadata is copied into the export location
                    if (t_description.unique()) {
                        t_location->metadata = t_description;
                    } else {
                        t_location->metadata = std::make_shared<misa_description_storage>(*t_description);
                    }

                    data->link("", t_location->metadata);
                    return;
                }

                std::cout << "[Cache] Creating " << t_location->internal_path() << " [" << t_location->external_path()
                          << "] as cache of type " << typeid(Cache).name() << std::endl;

                // Create the directory if necessary
                if (!boost::filesystem::exists(t_location->external_path())) {
                    std::cout << "[Cache] Creating directory " << t_location->external_path() << std::endl;
                    boost::filesystem::create_directories(t_location->external_path());
                }

                // Metadata is copied into the export location
                if (t_description.unique()) {
                    t_location->metadata = t_description;
                } else {
                    t_location->metadata = std::make_shared<misa_description_storage>(*t_description);
                }

                data->link(t_location->external_path(), t_location->metadata);
            }
        }

        /**
         * Links this cache to an imported filesystem path.
         * This path must exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         */
        void suggest_import_location(const misa_filesystem &t_filesystem, const boost::filesystem::path &t_path) {
            if (!data) {
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
                                     const boost::filesystem::path &t_path,
                                     const std::shared_ptr<misa_description_storage> &t_description) {
            if (!data) {
                if (t_filesystem.exported->has_subpath(t_path))
                    throw std::runtime_error(
                            "Suggested export location exported/" + t_path.string() + " is already used!");
                suggest_create(t_filesystem.exported->access(t_path), t_description);
            }
        }

        /**
         * Returns a description of the current cache
         * @return
         */
        std::shared_ptr<misa_description_storage> describe() const override {
            return data->describe();
        }

        /**
         * Thread-safe read-only access to the data.
         * @return
         */
        readonly_access <value_type> access_readonly() const {
            return readonly_access<value_type>(*data);
        }

        /**
        * Thread-safe read & write access to the data.
        * Other threads are blocked from access.
        * @return
        */
        readwrite_access <value_type> access_readwrite() {
            return readwrite_access<value_type>(*data);
        }

        /**
         * Thread-safe write-only access to the data.
         * Other threads are blocked from access.
         * @return
         */
        write_access <value_type> access_write() {
            return write_access<value_type>(*data);
        }

        /**
         * Returns the location of the cache.
         * This is the folder that contains the data. Please note that
         * this location might not be unique for different caches, as multiple caches might be created
         * on the same location.
         * Use get_unique_location() to find the actual file instead.
         * @return
         */
        boost::filesystem::path get_location() const {
            return data->get_location();
        }

        /**
         * Returns the unique location of the cache.
         * This usually points to the actual file containing the data
         * @return
         */
        boost::filesystem::path get_unique_location() const {
            return data->get_unique_location();
        }

    };
}