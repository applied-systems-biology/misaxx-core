//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_worker.h"
#include "misa_module_declaration_base.h"
#include "misa_module_base.h"
#include "misa_submodule.h"

namespace misaxx {

    /**
     * Second part of a MISA++ module. This module dispatcher is responsible dispatching the module-internal workers,
     * setting up the data and submodules.
     *
     * A MISA module must be always instantiated with a module definition (the root module is instantiated with a default definition)
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration>
    struct misa_module : public misa_module_base, public misa_worker<ModuleDeclaration>, public pattxx::dispatcher, public ModuleDeclaration {

    public:

        using module_definition_type = ModuleDeclaration;

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "misa_module only accepts module definitions as template parameter!");

        explicit misa_module(pattxx::nodes::node *t_node, ModuleDeclaration definition) :
                pattxx::dispatcher(t_node),
                ModuleDeclaration(std::move(definition)) {
        }

    protected:

        /**
         * Initializes a module data member
         * @tparam Data misa_module_data or misa_submodule instance
         * @param t_data
         * @return
         */
        template<class Data> Data& init_data(Data &t_data) {
            t_data.init(*this);
            return t_data;
        }

        /**
         * pattxx::dispatcher::dispatch with the additional function of setting the module accordingly.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance, typename... Args>
        Instance &misa_dispatch(const std::string &t_name, Args &&... args) {
            auto &inst = dispatch<Instance>(t_name, static_cast<ModuleDeclaration *>(this), std::forward<Args>(args)...);
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
            return dispatch<Module>(t_submodule.name, std::move(t_submodule.definition()));
        }

        /**
         * Returns the module definition
         * @return
         */
        ModuleDeclaration &module() override {
            return *this;
        }

    };
}
