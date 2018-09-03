//
// Created by rgerst on 03.09.18.
//


#pragma once

#include "misaxx/misa_module_definition.h"
#include "misaxx/misa_module.h"

namespace misaxx {

    struct misa_default_root_module_definition : public misa_module_definition {
    };

    /**
     * The default root module used by the MISA++ runtime.
     * Distributes all folders in the filesystem to the SubModule.
     * Files are ignored.
     */
    template<class SubModule> struct misa_default_root_module : public misa_module<misa_default_root_module_definition> {
        using misa_module::misa_module;

        void init() {
            for(const auto &ptr : *filesystem.imported) {
                filesystem::folder as_folder = std::dynamic_pointer_cast<filesystem::vfs_folder>(ptr.second);
                if(as_folder) {
                    misa_submodule<SubModule> module(*this, ptr.first, metadata());
                    module.init(*this);
                    misa_dispatch(module);
                }
            }
        }
    };

}
