//
// Created by rgerst on 03.09.18.
//


#pragma once

#include <misaxx/core/misa_module_interface.h>
#include <misaxx/core/misa_module.h>
#include <misaxx/core/misa_root_module_base.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>
#include <misaxx/core/runtime/misa_parameter_registry.h>

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

        void create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) override {
            if (misaxx::runtime_properties::is_simulating()) {
                t_blueprints.add(create_submodule_blueprint<SubModule>("__OBJECT__"));
                m_objects.push_back("__OBJECT__");
            } else {
                std::cout << "[multiobject_root] Dispatching root module for all input objects ..." << "\n";
                const nlohmann::json &object_json = misaxx::parameter_registry::get_parameter_json()["samples"];
                for (nlohmann::json::const_iterator it = object_json.begin(); it != object_json.end(); ++it) {
                    const std::string &name = it.key();
                    filesystem::entry e = filesystem.imported->access(name);
                    if (e->has_external_path()) {
                        if (boost::filesystem::is_directory(e->external_path())) {
                            std::cout << "[multiobject_root] Found object " << name << ". External path "
                                      << e->external_path().string() << " is valid." << "\n";
                            t_blueprints.add(create_submodule_blueprint<SubModule>(name));
                            m_objects.push_back(name);
                        } else {
                            std::cout << "[multiobject_root] Found object " << name << ", but external path "
                                      << e->external_path().string() << " does not exist. Skipping." << "\n";
                        }
                    } else {
                        std::cout << "[multiobject_root] Found object " << name
                                  << ", but it has no external path. Skipping." << "\n";
                    }
                }
            }
        }

        void create_parameters(misa_parameter_builder &t_parameters) override {

        }

        void build(const blueprint_builder &t_builder) override {
            for(const std::string &key : m_objects) {
                t_builder.build<SubModule>(key);
            }
        }

//        void misa_init() override {
//
//            // Only consider objects defined in the "objects" parameters
//            std::cout << "[multiobject_root] Dispatching root module for all input objects ..." << "\n";
//            const nlohmann::json &object_json = misa_runtime_base::instance().get_parameter_json()["objects"];
//            for(nlohmann::json::const_iterator it = object_json.begin(); it != object_json.end(); ++it){
//                const std::string &name = it.key();
//                filesystem::entry e = filesystem.imported->access(name);
//                if(e->has_external_path()) {
//                    if(boost::filesystem::is_directory(e->external_path())) {
//                        std::cout << "[multiobject_root] Found object " << name << ". External path " << e->external_path().string() << " is valid." << "\n";
//                        misa_submodule<SubModule> module;
//                        init_submodule(module, name);
//                        misa_dispatch(future_dispatch(module));
//                    }
//                    else {
//                        std::cout << "[multiobject_root] Found object " << name << ", but external path " << e->external_path().string() << " does not exist. Skipping." << "\n";
//                    }
//                }
//                else {
//                    std::cout << "[multiobject_root] Found object " << name << ", but it has no external path. Skipping." << "\n";
//                }
//            }
//        }
    private:
        std::vector<std::string> m_objects;
    };



}
