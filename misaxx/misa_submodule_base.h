//
// Created by rgerst on 13.12.18.
//

#pragma once

#include <misaxx/filesystem/misa_filesystem.h>

namespace misaxx {
    /**
     * Base class for a submodule data
     */
    struct misa_submodule_base {
        std::string name;

        /**
         * Returns the filesystem
         * @return
         */
        virtual misa_filesystem &get_filesystem() = 0;

        /**
         * Returns true if the submodule is instantiated
         * @return
         */
        virtual bool has_instance() const = 0;
    };
}




