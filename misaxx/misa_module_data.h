//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <pattxx/metadata.h>
#include "misa_module_definition_base.h"

namespace misaxx {
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
         * If true, this data object already contains valid data
         */
        bool has_value = false;

        misa_module_data() = delete;

        explicit misa_module_data(misa_module_definition_base &t_module, std::string t_name, pattxx::metadata t_metadata) :
        name(std::move(t_name)),
        metadata(std::move(t_metadata)) {

        }

        misa_module_data(misa_module_data &&other) = default;

        misa_module_data(const misa_module_data &other) = delete;
    };
}
