//
// Created by rgerst on 14.12.18.
//

#pragma once
#include <misaxx/core/runtime/misa_runtime.h>

namespace misaxx {

    /**
     * Internal implementation of the runtime that runs the tasks
     */
    struct misa_runtime;

    struct misa_cli_impl;

    /**
     * Base class for a CLI
     */
    struct misa_cli : public misa_runtime {

    private:

        misa_cli_impl *m_pimpl;

    public:

        /**
        * Return states of prepare()
        */
        enum class cli_result {
            continue_with_workload,
            no_workload,
            error,
            ok
        };

        misa_cli();

        virtual ~misa_cli();

    public:

        /**
         * Sets the module info
         * Must be done by the main() method
         */
        using misa_runtime::set_module_info;

        /**
         * Sets the root module
         * @tparam Module
         */
        template<class Module>
        void set_root_module(const std::string &t_name);

        /**
         * Runs the CLI
         * @param argc
         * @param argv
         */
        int prepare_and_run(int argc, const char** argv);

    private:

        /**
         * Loads parameters from CLI
         * @param argc
         * @param argv
         * @return
         */
        cli_result load_from_cli(int argc, const char** argv);

        /**
         * Runs the internal runtime
         * @return
         */
        cli_result run();
    };
}

#include <misaxx/core/runtime/detail/misa_cli.h>




