//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/task.h>
#include "misa_worker.h"
#include "misa_module_definition.h"

namespace misaxx {
    template<class ModuleDefinition> struct misa_task : public pattxx::task, public misa_worker<ModuleDefinition> {

        static_assert(std::is_base_of<misa_module_definition, ModuleDefinition>::value, "Template argument must be a module definition!");

        explicit misa_task(pattxx::nodes::node *t_node, ModuleDefinition *t_module) : pattxx::task(t_node), m_module(t_node) {

        }

        const ModuleDefinition &module() const override {
            return *m_module;
        }

    private:

        ModuleDefinition *m_module;

    };
}
