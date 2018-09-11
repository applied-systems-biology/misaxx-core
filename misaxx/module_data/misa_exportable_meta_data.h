//
// Created by rgerst on 11.09.18.
//


#pragma once

#include "../metadata/misa_metadata.h"
#include "../misa_module_data.h"
#include "misa_json_file.h"

namespace misaxx {

    /**
     * Module data class that only stores metadata.
     */
    struct misa_exportable_meta_data : public misa_file {
        using misa_file::misa_file;

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
                const std::any &v = kv.second;

                const auto *md = std::any_cast<const misa_metadata *>(v);
                if(md) {
                    md->write_to_json_dict(json);
                }

            }

            std::ofstream sw;
            sw.open(path.string());
            sw << json;
        }
    };
}