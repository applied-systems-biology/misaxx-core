//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <cxxh/containers/dynamic_singleton_map.h>
#include "misaxx/misa_module_declaration_base.h"
#include "../vdata/misa_metadata.h"

namespace misaxx {

    /**
     * Base class for data contained in a MISA++ module definition
     */
    struct misa_pdata {

        /**
         * Additional metadata attached by algorithms
         */
        cxxh::containers::dynamic_singleton_map<misa_metadata> user_metadata;

        /**
         * Returns the data string that indicates how a filesystem entry is interpreted
         * @return
         */
        virtual std::string dataString() = 0;

    };
}
