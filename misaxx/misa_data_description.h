//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * A description contains all necessary information to further process some data.
     * During importing, it is produced using a pattern.
     */
    struct misa_data_description : public misa_serializeable {
    };
}
