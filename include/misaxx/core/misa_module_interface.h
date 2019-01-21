//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <misaxx/core/filesystem/misa_filesystem.h>

namespace misaxx {

    /**
     * The module interface contains which data is available inside the module, which data is exported and
     * which modules are included.
     * The module definition is combined with the module dispatcher (misaxx::misa_module) to build the
     * final module.
     */
    struct misa_module_interface {
        /**
        * Filesystem of this module
        */
        misa_filesystem filesystem;

        /**
         * Initializes the data and sets them into relation with each other
         */
        virtual void setup() = 0;
    };
}
