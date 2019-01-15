//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <misaxx/misa_worker.h>
#include <misaxx/misa_parameter.h>

namespace misaxx {

    /**
     * Base class for a MISA++ task
     * @tparam ModuleDeclaration
     */
    struct misa_task : public misa_worker {

        using parameter_list = misa_parameter_builder;
        template<typename T> using parameter = misa_parameter<T>;

        /**
         * Parameter that determines if this task is parallelizeable
         */
        misa_parameter<bool> is_parallelizeable_parameter;

        using misa_worker::misa_worker;

    public:

        virtual void work() = 0;

        virtual void simulate_work();

        void create_parameters(parameter_list &t_parameters) override;

        void execute_work() override;

        bool is_parallelizeable() const override;

        const misa_parameter_builder &get_parameters() const override;

    private:

        std::unique_ptr<misa_parameter_builder> m_parameter_builder;

    };
}
