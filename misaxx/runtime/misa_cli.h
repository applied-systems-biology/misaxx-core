//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <string>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <misaxx/filesystem/misa_filesystem_empty_importer.h>
#include <misaxx/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/filesystem/misa_filesystem_json_importer.h>
#include <misaxx/runtime/misa_runtime.h>
#include <misaxx/misa_multiobject_root.h>

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
                    ("threads,t", po::value<int>(), "Sets the number of threads")
                    ("no-skip", "Disables skipping of work that already has been finished")
                    ("write-parameter-schema", po::value<std::string>(), "Writes a parameter schema to the target file");

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
            if(vm.count("write-parameter-schema")) {
                m_runtime.build_schema = true;
                m_runtime.simulation_mode = true;
                m_parameter_schema_path = vm["write-parameter-schema"].as<std::string>();
                if(!m_parameter_schema_path.parent_path().empty())
                    boost::filesystem::create_directories(m_parameter_schema_path.parent_path());
            }
            if(vm.count("threads")) {
                if(!m_runtime.build_schema) {
                    m_runtime.num_threads = vm["threads"].as<int>();
                }
                else {
                    m_runtime.num_threads = 1;
                    std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This application will run only with 1 thread." << std::endl;
                }
            }
            if(vm.count("no-skip")) {
                if(!m_runtime.build_schema) {
                    m_runtime.enable_skipping = false;
                }
            }
            if(vm.count("parameters")) {
                std::cout << "<#> <#> Loading parameters from " << vm["parameters"].as<std::string>() << std::endl;
                std::ifstream in(vm["parameters"].as<std::string>());
                in >> m_runtime.parameters;
            }
            else if(!m_runtime.build_schema) {
                throw std::runtime_error("You have to provide a parameter file!");
            }

            // Load runtime parameters that are not from CLI
            if(!vm.count("threads") && !m_runtime.build_schema) {
                m_runtime.num_threads = m_runtime.template get_json_or<int>({ "runtime", "num-threads" }, 1, pattxx::json::json_property<int>());
            }
            if(!vm.count("no-skip") && !m_runtime.build_schema) {
                m_runtime.enable_skipping = !m_runtime.template get_json_or<bool>({ "runtime", "no-skip" }, false, pattxx::json::json_property<bool>());
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
            if(m_runtime.build_schema) {
                std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This will build a parameter schema, but no real work is done!" << std::endl;
            }
            m_runtime.run();
            if(m_runtime.build_schema) {

                pattxx::json::json_schema_builder &schema = m_runtime.parameter_schema;

                // Save filesystem to parameter schema
                to_json_schema(m_runtime.instance().filesystem, schema);
                schema.insert_static<std::string>({"filesystem", "source"}, "json", pattxx::json::json_property<std::string>());

                // Workaround: Due to inflexibility with schema generation, manually put "__OBJECT__" nodes into list builders
                // /properties/algorithm -> nothing to do
                // /properties/objects/properties/__OBJECT__ -> /properties/objects/additionalProperties
                {
                    auto &base = schema.data["properties"]["objects"];
                    base["additionalProperties"] = std::move(base["properties"]["__OBJECT__"]);
                    base["properties"].erase(base["properties"].find("__OBJECT__"));
                }

                // /properties/runtime::filesystem/properties/json-data/properties/imported/properties/children/properties/__OBJECT__ -> /properties/runtime::filesystem/properties/json-data/properties/imported/properties/children/additionalProperties
                {
                    auto &base = schema.data["properties"]["filesystem"]["properties"]["json-data"]["properties"]["imported"]["properties"]["children"];
                    base["additionalProperties"] = std::move(base["properties"]["__OBJECT__"]);
                    base["properties"].erase(base["properties"].find("__OBJECT__"));
                }
                {
                    auto &base = schema.data["properties"]["filesystem"]["properties"]["json-data"]["properties"]["exported"]["properties"]["children"];
                    base["additionalProperties"] = std::move(base["properties"]["__OBJECT__"]);
                    base["properties"].erase(base["properties"].find("__OBJECT__"));
                }

                // Write runtime parameters
                schema.insert_optional<int>({"runtime", "num-threads"}, 1, pattxx::json::json_property<int>("Number of threads", ""));
                schema.insert_optional<bool>({"runtime", "no-skip"}, false, pattxx::json::json_property<bool>("Disable skipping", ""));

                std::cout << "<#> <#> Writing parameter schema to " << m_parameter_schema_path.string() << std::endl;
                m_runtime.parameter_schema.write(m_parameter_schema_path);
            }
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

        boost::filesystem::path m_parameter_schema_path;

        void load_filesystem() {

            // Do no importing if we build a schema
            if(m_runtime.build_schema) {
                misa_filesystem_empty_importer importer;
                get_runtime().get_filesystem() = importer.import();
                return;
            }

            // Otherwise load importer from parameters
            nlohmann::json &params = m_runtime.parameters["filesystem"];
            if(params["source"] == "directories") {
                misa_filesystem_directories_importer importer;
                importer.input_path = params["input-directory"].get<std::string>();
                importer.output_path = params["output-directory"].get<std::string>();
                get_runtime().get_filesystem() = importer.import();
            }
            else if(params["source"] == "json") {
                misa_filesystem_json_importer importer;
                if(params.find("json-data") != params.end()) {
                    importer.input_json = params["json-data"];
                }
                else {
                    importer.json_path = params["json-path"].get<std::string>();
                }
                get_runtime().get_filesystem() = importer.import();
            }
            else {
                throw std::runtime_error("Unknown filesystem type " + params["source"].get<std::string>());
            }
        }

    };
}