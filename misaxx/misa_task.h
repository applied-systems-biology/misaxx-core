//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/task.h>
#include "misa_worker.h"
#include "misa_module_definition_base.h"
#include "misa_data_ptr.h"

namespace misaxx {
    template<class ModuleDefinition> struct misa_task : public pattxx::task, public misa_worker<ModuleDefinition> {

        static_assert(std::is_base_of<misa_module_definition_base, ModuleDefinition>::value, "Template argument must be a module definition!");

        template<class Data> using misa_data = misa_data_ptr<Data>;

        explicit misa_task(pattxx::nodes::node *t_node, ModuleDefinition *t_module) : pattxx::task(t_node), m_module(t_module) {

        }

        ModuleDefinition &module() override {
            return *m_module;
        }

    private:

        ModuleDefinition *m_module;

    };
}
