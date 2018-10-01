//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <cxxh/containers/dynamic_singleton_map.h>
#include "misa_module_declaration_base.h"
#include "metadata/misa_metadata.h"

namespace misaxx {

    /**
     * Base class for data contained in a MISA++ module definition
     */
    struct misa_module_data {

        /**
         * Additional metadata attached by algorithms
         */
        cxxh::containers::dynamic_singleton_map<misa_metadata> user_metadata;

    };
}
