//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <string>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "filesystem/importers/directories_importer.h"
#include "filesystem/importers/json_importer.h"
#include "misa_runtime.h"

namespace misaxx {

    /**
     * MISA++ CLI
     * @tparam Module The module that should be executed
     * @tparam Runtime
     */
    template<class Module, class Runtime = misa_runtime<Module>> class misa_cli {

    public:

        explicit misa_cli(const std::string &t_root_name) : m_runtime(Runtime(t_root_name)) {

        }

        /**
         * Loads the parameters into the runtime.
         * @param argc
         * @param argv
         * @return
         */
        int prepare(const int argc, const char** argv) {
            namespace po = boost::program_options;

            po::options_description general_options("Runtime options");
            general_options.add_options()
                    ("help,h", "Help screen")
                    ("parameters,p", po::value<std::string>(), "Provides the list of parameters")
                    ("threads,t", po::value<int>()->default_value(1), "Sets the number of threads")
                    ("no-skip", "Disables skipping of work that already has been finished");

            po::command_line_parser parser(argc, argv);
            parser.options(general_options);
            po::parsed_options parsed_options = parser.run();

            po::variables_map vm;
            po::store(parsed_options, vm);
            po::notify(vm);

            if(vm.count("help")) {
                std::cout << general_options << std::endl;
                return 1;
            }
            if(vm.count("threads")) {
                m_runtime.num_threads = vm["threads"].as<int>();
            }
            if(vm.count("no-skip")) {
                m_runtime.enable_skipping = false;
            }
//            if(vm.count("write-parameter-schema")) {
//                m_runtime.build_schema = true;
//                m_parameter_schema_path = vm["write-parameter-schema"].as<std::string>();
//                boost::filesystem::create_directories(m_parameter_schema_path.parent_path());
//            }
            if(vm.count("parameters")) {
                std::cout << "<#> <#> Loading parameters from " << vm["parameters"].as<std::string>() << std::endl;
                std::ifstream in(vm["parameters"].as<std::string>());
                in >> m_runtime.parameters;
            }
            else {
                throw std::runtime_error("You have to provide a parameter file!");
            }

            load_filesystem();

            return 0;
        }

        /**
         * Runs the runtime and additional actions like writing parameter schemas
         */
        void run() {
            std::cout << "<#> <#> Starting run with " << m_runtime.num_threads << " threads" << std::endl;
            if(m_runtime.enable_skipping) {
                std::cout << "<#> <#> Skipping is enabled" << std::endl;
            }
            else {
                std::cout << "<#> <#> Skipping is disabled" << std::endl;
            }
//            if(m_runtime.build_schema) {
//                std::cout << "<#> <#> Parameter schema will be built. Please note that this can reduce the performance." << std::endl;
//            }
            m_runtime.run();
//            if(m_runtime.build_schema) {
//                std::cout << "<#> <#> Writing parameter schema to " << m_parameter_schema_path.string() << std::endl;
//                m_runtime.parameter_schema.write(m_parameter_schema_path);
//            }
        }

        /**
         * Loads the CLI parameters and if successful, runs the runtime.
         * @param argc
         * @param argv
         * @return
         */
        int prepare_and_run(const int argc, const char** argv) {
            int ret = prepare(argc, argv);
            if(ret != 0)
                return ret;
            run();
            return 0;
        }

        /**
         * Returns the internal runtime instance
         * @return
         */
        Runtime &get_runtime() {
            return m_runtime;
        }

        /**
         * Returns the internal runtime instance
         * @return
         */
        const Runtime &get_runtime() const {
            return m_runtime;
        }

    private:
        Runtime m_runtime;

        void load_filesystem() {
            nlohmann::json &params = m_runtime.parameters["runtime::filesystem"];
            if(params["source"] == "directories") {
                filesystem::importers::directories_importer importer;
                importer.input_path = params["input-directory"].get<std::string>();
                importer.output_path = params["output-directory"].get<std::string>();
                get_runtime().get_filesystem() = importer.import();
            }
            else if(params["source"] == "json") {
                filesystem::importers::json_importer importer;
                importer.json_path = params["json-path"].get<std::string>();
                get_runtime().get_filesystem() = importer.import();
            }
            else {
                throw std::runtime_error("Unknown filesystem type " + params["source"].get<std::string>());
            }
        }

    };
}