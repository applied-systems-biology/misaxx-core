//
// Created by rgerst on 11.09.18.
//


#pragma once

#include "misa_cached_data.h"
#include "misa_unsafe_json_file.h"

namespace misaxx {

    /**
     * Module data class that only stores metadata.
     * This is legacy. Don't use
     */
    struct [[deprecated]] misa_unsafe_exportable_meta_data : public misa_unsafe_file {

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-exportable-metadata";

        using misa_unsafe_file::misa_unsafe_file;

        /**
         * Metadata
         */
        cxxh::containers::dynamic_singleton_map<misa_metadata> user_metadata;

        /**
         * Saves the metadata included in this instance to the target JSON file
         */
        void save() {
            nlohmann::json json;
            for(const auto &kv : user_metadata) {
                const misa_metadata *md = kv.second.get();
                md->to_json(json[md->get_name()]);
            }

            std::ofstream sw;
            sw.open(get().string());
            sw << std::setw(4) << json;
        }
    };
}