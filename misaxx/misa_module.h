//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "misaxx/workers/misa_worker.h"
#include "misa_module_declaration_base.h"
#include "misa_module_base.h"
#include "misa_submodule.h"
#include "misaxx/workers/paths/algorithm_node_path.h"
#include "misaxx/workers/paths/object_node_path.h"
#include "misa_future_dispatch.h"
#include <misaxx/workers/misa_dispatcher.h>

namespace misaxx {

    /**
     * Second part of a MISA++ module. This module dispatcher is responsible dispatching the module-internal workers,
     * setting up the data and submodules.
     *
     * A MISA module must be always instantiated with a module definition (the root module is instantiated with a default definition)
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration>
    struct misa_module : public misa_module_base,
                         public misa_dispatcher<ModuleDeclaration>,
                         public ModuleDeclaration,
                         public std::enable_shared_from_this<ModuleDeclaration> {

    public:

        using module_declaration_type = ModuleDeclaration;
        template<class Instance> using dispatched = misa_future_dispatch<misa_dispatcher<ModuleDeclaration>, Instance>;

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "misa_module only accepts module definitions as template parameter!");

        explicit misa_module(const std::shared_ptr<nodes::misa_work_node> &t_node, ModuleDeclaration definition) :
                misa_dispatcher<ModuleDeclaration>(t_node, std::shared_ptr<ModuleDeclaration>()),
                ModuleDeclaration(std::move(definition)) {
        }

    protected:

        template<class FutureDispatch, class Instance = typename FutureDispatch::result_type>
        Instance &misa_dispatch(const FutureDispatch &t_dispatch) {
            return t_dispatch.dispatch_specific(*this);
        }

        std::shared_ptr<ModuleDeclaration> module() final {
            return std::enable_shared_from_this<ModuleDeclaration>::shared_from_this();
        }

    };
}
