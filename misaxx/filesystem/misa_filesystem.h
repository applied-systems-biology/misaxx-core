//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_filesystem_entry.h"

namespace misaxx {
    /**
     * Filesystem of a MISA++ module. The filesystem has 3 folders, "imported", "exported" and "modules".
     */
    struct misa_filesystem {
        /**
         * Contains all imported data
         */
        filesystem::entry imported;
        /**
         * Contains all exported / output data
         */
        filesystem::entry exported;

        /**
         * Returns true if this filesystem is valid
         * @return
         */
        bool is_valid() const {
            return imported && exported;
        }

        /**
         * Creates a sub-filesystem
         * @param t_name
         * @return
         */
        misa_filesystem create_subsystem(const std::string &t_name) {
            if(!is_valid())
                throw std::runtime_error("Cannot create sub-filesystem from invalid filesystem!");
            misa_filesystem result;
            result.imported = imported->access(t_name);
            result.exported = exported->access(t_name);
            return result;
        }
    };
}
