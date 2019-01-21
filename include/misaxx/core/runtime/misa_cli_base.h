//
// Created by rgerst on 14.12.18.
//

#pragma once

#include <misaxx/core/runtime/misa_runtime_base.h>

namespace misaxx {

    /**
     * Base class for a CLI
     */
    struct misa_cli_base {
    private:

        std::shared_ptr<misa_runtime_base> m_runtime;
        boost::filesystem::path m_parameter_schema_path;

    public:

        /**
         * Return states of prepare()
         */
        enum class cli_result {
            continue_with_workload,
            no_workload,
            error
        };

        explicit misa_cli_base(std::shared_ptr<misa_runtime_base> t_runtime) : m_runtime(std::move(t_runtime)) {

        }

        /**
         * Loads the parameters into the runtime.
         * @param argc
         * @param argv
         * @return
         */
        cli_result prepare(const int argc, const char** argv);

        /**
         * Runs the runtime and additional actions like writing parameter schemas
         */
        void run();

        /**
         * Loads the CLI parameters and if successful, runs the runtime.
         * @param argc
         * @param argv
         * @return
         */
        int prepare_and_run(const int argc, const char** argv);

        /**
         * Returns the runtime that this CLI is working on
         * @return
         */
        std::shared_ptr<misa_runtime_base> get_runtime() const;

    private:

        void load_filesystem();

    };
}




