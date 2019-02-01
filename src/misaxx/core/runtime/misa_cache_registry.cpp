//
// Created by rgerst on 16.01.19.
//

#include <misaxx/core/runtime/misa_cache_registry.h>
#include <src/misaxx/core/runtime/misa_runtime.h>
#include <misaxx/core/misa_cached_data_base.h>

using namespace misaxx;

void misaxx::cache_registry::register_cache(std::shared_ptr<misa_cache> t_cache) {
    misa_runtime::instance().register_cache(std::move(t_cache));
}

const std::unordered_set<std::shared_ptr<misa_cache>> &misaxx::cache_registry::get_registered_caches() {
    return misa_runtime::instance().get_registered_caches();
}

bool cache_registry::unregister_cache(const std::shared_ptr<misa_cache> &t_cache) {
    return misa_runtime::instance().unregister_cache(t_cache);
}

bool cache_registry::unregister_cache(const misa_cached_data_base &t_cache) {
    return unregister_cache(t_cache.get_cache_base());
}
