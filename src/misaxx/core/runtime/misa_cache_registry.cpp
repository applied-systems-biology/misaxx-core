//
// Created by rgerst on 16.01.19.
//

#include <misaxx/core/runtime/misa_cache_registry.h>
#include <src/misaxx/core/runtime/misa_runtime.h>

using namespace misaxx;

void misaxx::cache_registry::register_cache(std::shared_ptr<misa_cache> t_cache) {
    misa_runtime::instance().register_cache(std::move(t_cache));
}

const std::vector<std::shared_ptr<misa_cache>> &misaxx::cache_registry::get_registered_caches() {
    return misa_runtime::instance().get_registered_caches();
}
