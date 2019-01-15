//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <misaxx/misa_module_interface.h>
#include <misaxx/misa_module.h>
#include <misaxx/misa_root_module_base.h>
#include <misaxx/runtime/misa_runtime_base.h>

namespace misaxx {

    /**
     * Empty module definition for misa_multiobject_root
     */
    struct misa_multiobject_root_interface : public misa_module_interface {
        void setup() override {
        }
    };

    /**
     * Wrap the root module around this module to interpret subfolders inside the import filesystem
     * as objects. The SubModule is called on each of this objects.
     * @tparam SubModule
     */
    template<class SubModule>
    struct misa_multiobject_root : public misa_module<misa_multiobject_root_interface>,
                                   public misa_root_module_base {

        using misa_module<misa_multiobject_root_interface>::misa_module;

        void create_blueprints(blueprints &t_blueprints) override {
            if (misa_runtime_base::instance().is_simulating()) {
                t_blueprints.add(create_submodule_blueprint<SubModule>("__OBJECT__"));
                m_objects.push_back("__OBJECT__");
            } else {
                std::cout << "[multiobject_root] Dispatching root module for all input objects ..." << std::endl;
                const nlohmann::json &object_json = misa_runtime_base::instance().get_parameter_json()["objects"];
                for (nlohmann::json::const_iterator it = object_json.begin(); it != object_json.end(); ++it) {
                    const std::string &name = it.key();
                    filesystem::entry e = filesystem.imported->access(name);
                    if (e->has_external_path()) {
                        if (boost::filesystem::is_directory(e->external_path())) {
                            std::cout << "[multiobject_root] Found object " << name << ". External path "
                                      << e->external_path().string() << " is valid." << std::endl;
                            t_blueprints.add(create_submodule_blueprint<SubModule>(name));
                            m_objects.push_back(name);
                        } else {
                            std::cout << "[multiobject_root] Found object " << name << ", but external path "
                                      << e->external_path().string() << " does not exist. Skipping." << std::endl;
                        }
                    } else {
                        std::cout << "[multiobject_root] Found object " << name
                                  << ", but it has no external path. Skipping." << std::endl;
                    }
                }
            }
        }

        void build(const builder &t_builder) override {
            for(const std::string &key : m_objects) {
                t_builder.build<SubModule>(key);
            }
        }

//        void misa_init() override {
//
//            // Only consider objects defined in the "objects" parameters
//            std::cout << "[multiobject_root] Dispatching root module for all input objects ..." << std::endl;
//            const nlohmann::json &object_json = misa_runtime_base::instance().get_parameter_json()["objects"];
//            for(nlohmann::json::const_iterator it = object_json.begin(); it != object_json.end(); ++it){
//                const std::string &name = it.key();
//                filesystem::entry e = filesystem.imported->access(name);
//                if(e->has_external_path()) {
//                    if(boost::filesystem::is_directory(e->external_path())) {
//                        std::cout << "[multiobject_root] Found object " << name << ". External path " << e->external_path().string() << " is valid." << std::endl;
//                        misa_submodule<SubModule> module;
//                        init_submodule(module, name);
//                        misa_dispatch(future_dispatch(module));
//                    }
//                    else {
//                        std::cout << "[multiobject_root] Found object " << name << ", but external path " << e->external_path().string() << " does not exist. Skipping." << std::endl;
//                    }
//                }
//                else {
//                    std::cout << "[multiobject_root] Found object " << name << ", but it has no external path. Skipping." << std::endl;
//                }
//            }
//        }
    private:
        std::vector<std::string> m_objects;
    };



}
