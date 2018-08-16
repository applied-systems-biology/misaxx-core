//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <libs/pattxx/pattxx/metadata.h>
#include "misa_module_definition.h"

namespace misaxx {
    /**
     * Base class for data contained in a MISA++ module definition
     */
    struct misa_module_data {

        pattxx::metadata metadata;

        misa_module_data() = delete;

        explicit misa_module_data(misa_module_definition &definition, pattxx::metadata t_metadata) : metadata(std::move(t_metadata)) {

        }

        misa_module_data(const misa_module_data &other) = delete;
    };
}
