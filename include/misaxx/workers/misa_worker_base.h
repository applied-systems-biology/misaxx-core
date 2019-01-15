//
// Created by rgerst on 04.09.18.
//


#pragma once

#include <memory>

namespace misaxx {

    namespace nodes {
        struct misa_work_node;
    }

    struct misa_worker_base {

        virtual ~misa_worker_base() = default;

        /**
         * Returns the node that the worker is working on
         * @return
         */
        virtual std::shared_ptr<nodes::misa_work_node> get_node() const = 0;

        /**
       * Call this function to indicate that the work has been rejected (if for example an input does not exist, yet).
       * The node will queue this work for re-doing.
       * If you want to implement dependencies between workers, use the dependency functions of a dispatcher instead.
       */
        virtual void reject_work() = 0;

        /**
         * Runs the workload
         */
        virtual void execute_work() = 0;

        /**
         * Returns true if the worker allows parallelization
         * @return
         */
        virtual bool is_parallelizeable() const = 0;

        /**
         * Returns a shared pointer to the worker
         * @return
         */
        virtual std::shared_ptr<misa_worker_base> self() const = 0;
    };
}
