//
// Created by rgerst on 28.11.18.
//


#pragma once

namespace misaxx {
    /**
     * Convenience type for creating accessors.
     * @tparam Pattern The pattern that is associated to the underlying cache
     * @tparam Description The description that is associated to the underlying cache
     */
    template<class Pattern, class Description, class Derived> struct misa_default_description_accessors {
        const Pattern &get_data_pattern() const {
            return static_cast<const Derived*>(this)->describe()->template get<Pattern>();
        }

        const Description &get_data_description() const {
            return static_cast<const Derived*>(this)->describe()->template get<Description>();
        }
    };

    /**
     * Derives misa_default_description_accessors from a cache type
     */
    template<class Cache, class Derived> using misa_description_accessors_from_cache = misa_default_description_accessors<typename Cache::pattern_type, typename Cache::description_type, Derived>;
}
