//
// Created by rgerst on 03.09.18.
//


#pragma once

#include "misaxx/misa_module_declaration.h"
#include "misaxx/misa_module.h"
#include "misa_root_module_base.h"

namespace misaxx {

    /**
     * Empty module definition for misa_multiobject_root
     */
    struct misa_multiobject_root_declaration : public misa_module_declaration {
        void init_data() override {
        }
    };

    /**
     * Wrap the root module around this module to interpret subfolders inside the import filesystem
     * as objects. The SubModule is called on each of this objects.
     * @tparam SubModule
     */
    template<class SubModule> struct misa_multiobject_root : public misa_module<misa_multiobject_root_declaration>,
            public misa_root_module_base {
        using misa_module<misa_multiobject_root_declaration>::misa_module;

        void misa_init() override {
            boost::filesystem::directory_iterator it(filesystem.imported->external_path());
            while(it != boost::filesystem::directory_iterator()) {
                boost::filesystem::path external_path = *it++;
                if(boost::filesystem::is_directory(external_path)) {
                    filesystem::entry e = filesystem.imported->access(external_path.filename());
                    misa_submodule<SubModule> module;
                    init_submodule(module, external_path.filename().string());
                    misa_dispatch(future_dispatch(module));
                }
            }
        }

        void misa_simulate() override {
            misa_submodule<SubModule> module;
            init_submodule(module, "__OBJECT__");
            misa_dispatch(future_dispatch(module));
        }
    };

}
