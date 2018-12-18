//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/workers/misa_worker.h>
#include <misaxx/misa_module_declaration_base.h>
#include <misaxx/workers/paths/object_node_path.h>
#include <misaxx/workers/paths/algorithm_node_path.h>
#include <misaxx/workers/paths/full_node_path.h>

namespace misaxx {

    /**
     * Base class for a MISA++ task
     * @tparam ModuleDeclaration
     */
    template<class ModuleDeclaration> struct misa_task : public misa_worker<ModuleDeclaration> {

        /**
        * Indicates that the task can be run in parallel context. Defaults to true.
        */
        bool m_is_parallelizeable = misa_worker<ModuleDeclaration>::template from_algorithm_json_or<bool>("task::is_parallelizeable", true, misa_json_property<bool>().with_title("Is parallelizeable"));

//        /**
//       * Indicates that the task can be skipped. Defaults to true.
//       */
//        bool m_is_skippable = misa_worker<ModuleDeclaration>::template from_json_or<bool, full_node_path>("task::is_skippable", true, misa_json_property<bool>("Enable skipping", "If false, the task will not be skipped."));

        static_assert(std::is_base_of<misa_module_declaration_base, ModuleDeclaration>::value, "Template argument must be a module definition!");

        using misa_worker<ModuleDeclaration>::misa_worker;

    public:

        virtual void misa_work() = 0;

        virtual void misa_simulate() {
        }

        void work() override {
            if(misaxx::misa_runtime_base::instance().is_simulating())
                misa_simulate();
            else
                misa_work();
        }

        bool is_parallelizeable() const override {
            return m_is_parallelizeable;
        }

    };
}
