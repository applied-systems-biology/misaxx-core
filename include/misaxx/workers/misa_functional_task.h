//
// Created by rgerst on 01.08.18.
//


#pragma once

#include <misaxx/workers/misa_task.h>

namespace misaxx {

    /**
     * A tasks that runs some arbitrary function as workload.
     * Please keep in mind to manually set is_parallelizeable to false if the task interacts with another thread unless
     * additional measures are taken to prevent race conditions.
     *
     */
    template<class ModuleDeclaration> struct misa_functional_task : public misa_task<ModuleDeclaration> {

        using misa_task<ModuleDeclaration>::misa_task;
        using function_type = std::function<void (ModuleDeclaration&)>;

        function_type m_function;

        void misa_work() override {
            m_function(this->module());
        }

    };

}