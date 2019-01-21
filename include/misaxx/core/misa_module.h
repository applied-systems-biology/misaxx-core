//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_dispatch_blueprint.h>
#include <misaxx/core/misa_dispatcher.h>

namespace misaxx {

    /**
     * Second part of a MISA++ module. This module dispatcher is responsible dispatching the module-internal workers,
     * setting up the data and submodules.
     *
     * A MISA module must be always instantiated with a module interface (the root module is instantiated with a default interface)
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration>
    struct misa_module : public misa_dispatcher,
                         public ModuleDeclaration,
                         public std::enable_shared_from_this<ModuleDeclaration> {

    public:

        using module_interface_type = ModuleDeclaration;

        static_assert(std::is_base_of<misa_module_interface, ModuleDeclaration>::value, "misa_module only accepts module interfaces as template parameter!");

        explicit misa_module(const std::shared_ptr<misa_work_node> &t_node, ModuleDeclaration definition) :
                misa_dispatcher(t_node, std::shared_ptr<ModuleDeclaration>()),
                ModuleDeclaration(std::move(definition)) {
        }

        misa_worker::module get_module() override {
            return std::enable_shared_from_this<ModuleDeclaration>::shared_from_this();
        }

    };
}
