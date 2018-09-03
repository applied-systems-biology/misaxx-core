//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_module_definition_base.h"
#include "misa_worker.h"
#include "misa_module_base.h"
#include "misa_data_ptr.h"

namespace misaxx {

    /**
     * MISA++ dispatcher base class.
     * @tparam ModuleDefinition
     */
    template<class ModuleDefinition> struct misa_dispatcher : public pattxx::dispatcher, public misa_worker<ModuleDefinition> {
        static_assert(std::is_base_of<misa_module_definition_base, ModuleDefinition>::value, "Template argument must be a module definition!");

        template<class Data> using misa_data = misa_data_ptr<Data>;

        explicit misa_dispatcher(pattxx::nodes::node *t_node, ModuleDefinition *t_module) : pattxx::dispatcher(t_node), m_module(t_module) {

        }

        /**
         * Returns the module definition
         * @return
         */
        const ModuleDefinition &module() const override {
            return *m_module;
        }

    protected:

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
         * Dispatches a submodule as described in the module definition
         * @tparam Submodule
         * @tparam Module
         * @param m_submodule
         * @return
         */
        template<class Submodule, class Module = typename Submodule::module_type>
        Module &misa_dispatch(Submodule &t_submodule) {
            if(t_submodule.has_instance())
                throw std::runtime_error("The submodule already has been instantiated!");
            if(!t_submodule.definition().filesystem.is_valid())
                throw std::runtime_error("The submodule's filesystem is invalid! Please initialize it, first!");
            return dispatch<Module>(t_submodule.get_name(), std::move(t_submodule.definition()));
        }

    private:

        ModuleDefinition *m_module;
    };
}
