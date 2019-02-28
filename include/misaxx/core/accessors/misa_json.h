//
// Created by rgerst on 26.02.19.
//

#pragma once

#include <misaxx/core/caches/misa_json_cache.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/misa_default_description_accessors.h>

namespace misaxx {
    /**
     * Cache that allows access to JSON data
     */
    struct misa_json : public misa_cached_data<misa_json_cache>,
            public misa_description_accessors_from_cache<misa_json_cache, misa_json> {
        using  misa_cached_data<misa_json_cache>::misa_cached_data;

        /**
         * Returns true if the JSON file already exists
         * @return
         */
        bool exists() const;
    };
}




