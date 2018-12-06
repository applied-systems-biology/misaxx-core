//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <misaxx/misa_module_declaration.h>
#include <misaxx/misa_module.h>
#include <misaxx/misa_root_module_base.h>
#include <misaxx/runtime/misa_runtime_base.h>

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

            // Only consider objects defined in the "objects" parameters
            std::cout << "[multiobject_root] Dispatching root module for all input objects ..." << std::endl;
            const nlohmann::json &object_json = misa_runtime_base::instance().get_parameter_json()["objects"];
            for(nlohmann::json::const_iterator it = object_json.begin(); it != object_json.end(); ++it){
                const std::string &name = it.key();
                filesystem::entry e = filesystem.imported->access(name);
                if(e->has_external_path()) {
                    if(boost::filesystem::is_directory(e->external_path())) {
                        std::cout << "[multiobject_root] Found object " << name << ". External path " << e->external_path().string() << " is valid." << std::endl;
                        misa_submodule<SubModule> module;
                        init_submodule(module, name);
                        misa_dispatch(future_dispatch(module));
                    }
                    else {
                        std::cout << "[multiobject_root] Found object " << name << ", but external path " << e->external_path().string() << " does not exist. Skipping." << std::endl;
                    }
                }
                else {
                    std::cout << "[multiobject_root] Found object " << name << ", but it has no external path. Skipping." << std::endl;
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
