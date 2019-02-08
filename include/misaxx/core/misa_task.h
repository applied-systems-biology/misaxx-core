//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_parameter.h>

namespace misaxx {

    /**
     * Base class for a MISA++ task
     * @tparam ModuleDeclaration
     */
    struct misa_task : public misa_worker {
    public:

        using parameter_list = misa_parameter_builder;
        template<typename T> using parameter = misa_parameter<T>;

        /**
         * Parameter that determines if this task is parallelizeable
         * In contrast to other parameters, it can be queried before work() is called
         */
        misa_parameter<bool> is_parallelizeable_parameter;

        misa_task(const node &t_node, const module &t_module);

        /**
         * Function where the actual work should be done
         */
        virtual void work() = 0;

        /**
         * Called instead of work() if the runtime is in simulation modue
         */
        virtual void simulate_work();

        /**
         * Allows creation of parameters
         * @param t_parameters
         */
        void create_parameters(parameter_list &t_parameters) override;

        /**
         * Called by the runtime to execute the work
         */
        void execute_work() override;

        /**
         * Returns the value of the "task::is-parallelizable" parameter
         * @return
         */
        bool is_parallelizeable() const override;

        /**
         * Returns the parameter builder
         * @return
         */
        const misa_parameter_builder &get_parameters() const override;

    private:

        std::unique_ptr<misa_parameter_builder> m_parameter_builder;

    };
}
