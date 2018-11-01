//
// Created by rgerst on 01.11.18.
//


#pragma once

#include "../misa_metadata.h"

namespace misaxx {
    /**
     * A description contains all necessary information to further process some data.
     * During importing, it is produced using a pattern.
     */
    struct misa_description : public misa_metadata {

        /**
         * Internally used by the metadata serialization
         * @return
         */
        std::string get_name() const final {
            return "descriptions";
        }

    };
}
