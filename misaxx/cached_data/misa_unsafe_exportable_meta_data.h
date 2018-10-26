//
// Created by rgerst on 11.09.18.
//


#pragma once

#include "misa_cached_data.h"
#include "misa_unsafe_json_file.h"

namespace misaxx {

    /**
     * Module data class that only stores metadata.
     */
    struct misa_unsafe_exportable_meta_data : public misa_unsafe_file {

        /**
        * Used by the misa_cache_registry
        */
        static inline const std::string DATA_TYPE = "unsafe-exportable-metadata";

        using misa_unsafe_file::misa_unsafe_file;

        template<class Type>
        const Type &at() const {
            return user_metadata.at<Type>();
        }

        template<class Type>
        Type &at() {
            return user_metadata.at<Type>();
        }

        template<class Type>
        void insert(Type &&t_src) {
            user_metadata.insert(std::forward<Type>(t_src));
        }

        template<class Type>
        auto find() const {
            return user_metadata.find<Type>();
        }

        auto begin() {
            return user_metadata.begin();
        }

        auto begin() const {
            return user_metadata.begin();
        }

        auto end() {
            return user_metadata.end();
        }

        auto end() const {
            return user_metadata.end();
        }

        template<class Type>
        Type &access() {
            return user_metadata.access<Type>();
        }

        template<class Type> bool has() const {
            return user_metadata.has<Type>();
        }

        /**
         * Saves the metadata included in this instance to the target JSON file
         */
        void save() {
            nlohmann::json json;
            for(const auto &kv : user_metadata) {
                const misa_metadata *md = kv.second.get();
                json[md->get_name()] = md->to_json();
            }

            std::ofstream sw;
            sw.open(path.string());
            sw << std::setw(4) << json;
        }
    };
}