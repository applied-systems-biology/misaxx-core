//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_worker.h"
#include "misa_module_definition.h"
#include "misa_module_base.h"

namespace misaxx {

    /**
     * Second part of a MISA++ module. This module dispatcher is responsible dispatching the module-internal workers,
     * setting up the data and submodules.
     *
     * A MISA module must be always instantiated with a module definition (the root module is instantiated with a default definition)
     * @tparam ModuleDefinition
     */
    template<class ModuleDefinition>
    struct misa_module : public misa_module_base, public misa_worker<ModuleDefinition>, private pattxx::dispatcher, public ModuleDefinition {

    public:

        using module_type = ModuleDefinition;

        static_assert(std::is_base_of<misa_module_definition, ModuleDefinition>::value, "misa_module only accepts module definitions as template parameter!");

        explicit misa_module(pattxx::nodes::node *t_node, filesystem::folder t_filesystem) :
                pattxx::dispatcher(t_node) {
            if (t_node->get_parent() != nullptr)
                throw std::runtime_error("MISA++ module is initialized without a module definition. This is only valid for the root module.");
            this->get_filesystem() = std::move(t_filesystem);
        }

        explicit misa_module(pattxx::nodes::node *t_node, filesystem::folder t_filesystem, ModuleDefinition definition) :
                pattxx::dispatcher(t_node),
                ModuleDefinition(std::move(definition)) {
            this->get_filesystem() = std::move(t_filesystem);
        }

    protected:

        using pattxx::dispatcher::run_function;
        using pattxx::worker::path;
        using pattxx::worker::get_node;
        using pattxx::worker::parameters_exist;
        using pattxx::worker::input;
        using pattxx::worker::output;
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
        template<class Instance, typename... Args>
        Instance &misa_dispatch(const std::string &t_name, Args &&... args) {
            auto &inst = dispatch<Instance>(t_name, static_cast<ModuleDefinition *>(this), std::forward<Args>(args)...);
            if constexpr (std::is_base_of<misa_module_base, Instance>::value) {
                static_assert(sizeof...(Args) > 0, "You have to provide a module definition to the submodule!");
            }
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
        Module &misa_dispatch(Submodule &m_submodule) {
            return dispatch<Module>(m_submodule.get_name(), this->get_filesystem(), m_submodule.definition()); // TODO: Put module data in ./modules/<module name>
        }

        const ModuleDefinition &module() const override {
            return *this;
        }
//
//        filesystem::vfs_folder &vfs_import() {
//
//        }
//
//        filesystem::vfs_folder &vfs_export() {
//
//        }
//
//        filesystem::vfs_folder &vfs_modules() {
//
//        }


    };
}
