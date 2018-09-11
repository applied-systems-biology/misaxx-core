//
// Created by rgerst on 11.09.18.
//


#pragma once

#include "../misa_module_data.h"

namespace misaxx {

    /**
     * Module data class that only stores metadata.
     */
    struct misa_meta_data : public misa_module_data {
        using misa_module_data::misa_module_data;

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
    };
}