//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <misaxx/core/misa_multiobject_root.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/misa_module_info.h>
#include <misaxx/core/runtime/misa_cli_base.h>

namespace misaxx {

    /**
     * Creates a new runtime
     * @param t_module_info The module information of the current worker module
     * @param t_module_interface_builder A function that constructs a module interface for the worker module
     * @param t_module_dispatcher_builder A function that creates the module dispatcher from the interface
     * @return
     */
    extern std::shared_ptr<misa_runtime> create_runtime(misa_module_info t_module_info,
                                                        std::shared_ptr<misaxx::misa_module_interface>t_module_interface_builder,
                                                        std::function<std::shared_ptr<misa_dispatcher>(const std::shared_ptr<misa_work_node>&, std::shared_ptr<misa_module_interface>)> t_module_dispatcher_builder);

    /**
     * MISA++ CLI
     * @tparam Module The module that should be executed
     * @tparam Runtime
     */
    template<class Module> class misa_cli : public misa_cli_base {

    public:

        using module_type = Module;
        using root_module_type = misa_multiobject_root<module_type>;

        explicit misa_cli(misa_module_info t_module_info) :
            misa_cli_base(create_runtime(std::move(t_module_info),
                                         std::make_shared<typename Module::module_interface_type>(), [](const std::shared_ptr<misa_work_node>& nd, std::shared_ptr<misa_module_interface> interface) {
                return std::make_shared<Module>(nd, std::move(*std::dynamic_pointer_cast<typename Module::module_interface_type>(interface)));
            })) {

        }

    private:

        boost::filesystem::path m_parameter_schema_path;

    };
}