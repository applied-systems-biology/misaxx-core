//
// Created by rgerst on 09.08.18.
//


#pragma once

#include "misa_entry.h"

namespace misaxx::filesystem {

    /**
     * File in the MISA++ filesystem
     */
    class misa_file : public misa_entry {
    public:
        using misa_entry::misa_entry;

        bool empty() const override {
            return false;
        }
    };
}
