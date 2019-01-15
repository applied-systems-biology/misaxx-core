//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/workers/misa_worker.h>
#include <misaxx/workers/paths/object_node_path.h>
#include <misaxx/workers/paths/algorithm_node_path.h>
#include <misaxx/workers/paths/full_node_path.h>

namespace misaxx {

    /**
     * Base class for a MISA++ task
     * @tparam ModuleDeclaration
     */
    struct misa_task : public misa_worker {

        /**
        * Indicates that the task can be run in parallel context. Defaults to true.
        */
        bool m_is_parallelizeable = misa_worker::from_algorithm_json_or<bool>("task::is_parallelizeable", true, misa_json_property<bool>().with_title("Is parallelizeable"));

        using misa_worker::misa_worker;

    public:

        virtual void work() = 0;

        virtual void simulate_work();

        void execute_work() override;

        bool is_parallelizeable() const override;

    };
}
