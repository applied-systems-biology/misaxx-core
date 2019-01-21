//
// Created by rgerst on 16.01.19.
//


#pragma once

#include <memory>
#include <vector>

namespace misaxx {
    struct misa_cache;
}

namespace misaxx::cache_registry {
    extern void register_cache(std::shared_ptr<misa_cache> t_cache);
    extern const std::vector<std::shared_ptr<misa_cache>> &get_registered_caches();
}