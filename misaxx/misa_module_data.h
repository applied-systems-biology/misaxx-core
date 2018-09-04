//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <pattxx/metadata.h>
#include <cmake-build-debug/_deps/cxxh-src/cxxh/containers/singleton_map.h>
#include "misa_module_definition_base.h"

namespace misaxx {

    /**
     * Add this type to a MISA++ module data instance to reset its content
     */
    struct misa_clear_data {

    };

    /**
     * Base class for data contained in a MISA++ module definition
     */
    struct misa_module_data {

        /**
         * Name of this data
         */
        std::string name;

        /**
         * Additional metadata
         */
        pattxx::metadata metadata;

        /**
         * Additional metadata attached by algorithms
         */
        cxxh::containers::singleton_map user_metadata;

        /**
         * If true, this data object already contains valid data
         */
        bool has_value = false;

        /**
         * Pointer to the module. Set by the init method
         */
        misa_module_definition_base *m_module = nullptr;

        misa_module_data() = delete;

        explicit misa_module_data(misa_module_definition_base &t_module, std::string t_name, pattxx::metadata t_metadata = pattxx::metadata()) :
                name(std::move(t_name)),
                metadata(std::move(t_metadata)) {

        }

        misa_module_data(misa_module_data &&other) = default;

        misa_module_data(const misa_module_data &other) = delete;

        virtual void clear() {
            has_value = false;
        }

        virtual void operator <<(const misa_clear_data &t_data) {
            clear();
        }
    };
}
