//
// Created by rgerst on 10.08.18.
//


#pragma once
#include <memory>
#include <pattxx/metadata.h>
#include "misa_filesystem.h"

namespace misaxx {

    struct misa_module_data;


    struct misa_module_definition_base {
        /**
         * Filesystem of this module
         */
        misa_filesystem filesystem;
    };
}
