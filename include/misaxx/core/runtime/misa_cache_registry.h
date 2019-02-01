//
// Created by rgerst on 16.01.19.
//


#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

namespace misaxx {
    struct misa_cache;
    struct misa_cached_data_base;
}

namespace misaxx::cache_registry {
    /**
     * Registers a cache into the runtime
     * This will enable postprocessing of the cache
     * @param t_cache
     */
    extern void register_cache(std::shared_ptr<misa_cache> t_cache);

    /**
     * Removes a cache from the registry
     * @param t_cache
     * @return if successful
     */
    extern bool unregister_cache(const std::shared_ptr<misa_cache> &t_cache);

    /**
     * Removes a cache from the registry
     * @param t_cache
     * @return if successful
     */
    extern bool unregister_cache(const misa_cached_data_base &t_cache);

    /**
     * Returns the registered caches
     * @return
     */
    extern const std::unordered_set<std::shared_ptr<misa_cache>> &get_registered_caches();
}