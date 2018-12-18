//
// Created by rgerst on 10.08.18.
//


#pragma once
#include <memory>
#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/runtime/misa_runtime_base.h>

namespace misaxx {

    struct misa_module_declaration_base {

        /**
         * The current runtime
         */
        misa_runtime_base *m_runtime = nullptr;

        /**
         * Filesystem of this module
         */
        misa_filesystem filesystem;

        /**
         * Initializes the data and sets them into relation with each other
         */
        virtual void init_data() = 0;
    };
}
