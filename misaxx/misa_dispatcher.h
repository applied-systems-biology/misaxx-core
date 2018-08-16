//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_module_definition.h"
#include "misa_worker.h"
#include "misa_module_base.h"

namespace misaxx {
    template<class ModuleDefinition> struct misa_dispatcher : private pattxx::dispatcher, public misa_worker<ModuleDefinition> {
        static_assert(std::is_base_of<misa_module_definition, ModuleDefinition>::value, "Template argument must be a module definition!");

        explicit misa_dispatcher(pattxx::nodes::node *t_node, ModuleDefinition *t_module) : pattxx::dispatcher(t_node) {

        }

        const ModuleDefinition &module() const override {
            return *m_module;
        }

    protected:

        using pattxx::dispatcher::run_function;
        using pattxx::worker::path ;
        using pattxx::worker::get_node ;
        using pattxx::worker::parameters_exist ;
        using pattxx::worker::input ;
        using pattxx::worker::output ;
        using pattxx::worker::reject_work;
        using pattxx::worker::from_json_or;
        using pattxx::worker::from_json;
        using pattxx::worker::define;

        /**
         * pattxx::dispatcher::dispatch with the additional function of setting the module accordingly.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance, typename... Args> Instance &misa_dispatch(const std::string &t_name, Args&&... args) {
            auto &inst = dispatch<Instance>(t_name, static_cast<ModuleDefinition*>(this), std::forward<Args>(args)...);
            return inst;
        }

        /**
         * Dispatches a submodule
         * @tparam Submodule
         * @tparam Module
         * @param m_submodule
         * @return
         */
        template<class Submodule, class Module = typename Submodule::module_type> Module &misa_dispatch_submodule(Submodule &m_submodule) {

        }

    private:

        ModuleDefinition *m_module;
    };
}
