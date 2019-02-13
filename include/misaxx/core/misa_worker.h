//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_module_interface.h>
#include <misaxx/core/misa_parameter_builder.h>

namespace misaxx {

    struct misa_work_node;

    /**
     * Base class for a worker
     */
    struct misa_worker {

        using node = std::shared_ptr<misa_work_node>;
        using module = std::shared_ptr<misa_module_interface>;

        /**
         * Instantiates the worker
         * @param t_node
         * @param t_module
         */
        explicit misa_worker(const node &t_node, const module &t_module);

        /**
         * Returns a pointer to the module that this worker is working on
         * @return
         */
        virtual module get_module();

        /**
         * Returns the current module and casts it to the target type
         * @tparam Module
         * @return
         */
        template<class Module> std::shared_ptr<Module> get_module_as() {
            return std::dynamic_pointer_cast<Module>(get_module());
        }

        /**
         * Returns a pointer to itself
         * @return
         */
        std::shared_ptr<misa_worker> self() const;;

        /**
         * Gets the node that this worker is associated to
         * @return
         */
        std::shared_ptr<misa_work_node> get_node() const;


        /**
         * Rejects the workload. It will be tried again later.
         */
        void repeat_work();

        /**
         * Override this function to create misa_parameter<T> queries
         * @param t_parameters
         */
        virtual void create_parameters(misa_parameter_builder &t_parameters) = 0;

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
         * Returns the parameters
         * @return
         */
        virtual const misa_parameter_builder &get_parameters() const = 0;

    private:

        /**
         * The parent node the worker is connected to
         * Must be a raw pointer, as the  shared ptr is not ready during construction and we
         * get a bad_weak_ptr exception
         */
        std::weak_ptr<misa_work_node> m_node;

        /**
         * Pointer to the module interface
         */
        std::weak_ptr<misa_module_interface> m_module;
    };
}
