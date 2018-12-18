//
// Created by rgerst on 27.11.18.
//


#pragma once
#include <misaxx/patterns/misa_dummy_pattern.h>
#include <misaxx/misa_default_cache.h>

namespace misaxx {

    /**
     * Convenience cache type that is manually linked by other caches (e.g. sub-images) and therefor does not
     * require the pattern-description approach used by other caches.
     * This type of cache only requires a pattern
     * @tparam Cache
     * @tparam Description
     */
    template<class Cache, class Description> class misa_manual_cache : public misa_default_cache<Cache, misa_dummy_pattern<Description>, Description> {
    protected:
        Description produce_description(const boost::filesystem::path &t_location, const misa_dummy_pattern<Description> &t_pattern) override {
            // Manual linkage will create a proper description
            return Description();
        }
    };
}
