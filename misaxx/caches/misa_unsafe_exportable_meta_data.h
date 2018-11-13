//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <misaxx/misa_cached_data.h>
#include <misaxx/caches/misa_unsafe_json_file.h>

namespace misaxx {

    /**
     * Module data class that only stores metadata.
     * This is legacy. Don't use
     */
    struct [[deprecated]] misa_unsafe_exportable_meta_data : public misa_unsafe_file {

        using misa_unsafe_file::misa_unsafe_file;

        /**
         * Metadata
         */
        cxxh::containers::dynamic_singleton_map<misa_serializeable> user_metadata;

        /**
         * Saves the metadata included in this instance to the target JSON file
         */
        void save() {
            nlohmann::json json;
            for(const auto &kv : user_metadata) {
                const misa_serializeable *md = kv.second.get();
                md->to_json(json[md->get_serialization_id().get_id()]);
            }

            std::ofstream sw;
            sw.open(get().string());
            sw << std::setw(4) << json;
        }
    };
}