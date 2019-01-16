//
// Created by rgerst on 14.12.18.
//

#include <boost/program_options.hpp>
#include <misaxx/filesystem/misa_filesystem_empty_importer.h>
#include <misaxx/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/filesystem/misa_filesystem_json_importer.h>
#include <misaxx/misa_cached_data_base.h>
#include <misaxx/attachments/misa_location.h>
#include <misaxx-helpers/measurement/manual_stopwatch.h>
#include <misaxx/runtime/misa_parameter_registry.h>
#include "misaxx/runtime/misa_cli_base.h"

using namespace misaxx;

void misa_cli_base::load_filesystem() {

    // Do no importing if we build a schema
    if(m_runtime->is_simulating()) {
        misa_filesystem_empty_importer importer;
        m_runtime->set_filesystem(importer.import());
        return;
    }

    // Otherwise load importer from parameters
    nlohmann::json &params = m_runtime->get_parameter_json()["filesystem"];
    if(params["source"] == "directories") {
        misa_filesystem_directories_importer importer;
        importer.input_path = params["input-directory"].get<std::string>();
        importer.output_path = params["output-directory"].get<std::string>();
        m_runtime->set_filesystem(importer.import());
    }
    else if(params["source"] == "json") {
        misa_filesystem_json_importer importer;
        if(params.find("json-data") != params.end()) {
            importer.input_json = params["json-data"];
        }
        else {
            importer.json_path = params["json-path"].get<std::string>();
        }
        m_runtime->set_filesystem(importer.import());
    }
    else {
        throw std::runtime_error("Unknown filesystem type " + params["source"].get<std::string>());
    }
}

int misa_cli_base::prepare(const int argc, const char **argv) {
    namespace po = boost::program_options;

    po::options_description general_options("Runtime options");
    general_options.add_options()
            ("help,h", "Help screen")
            ("parameters,p", po::value<std::string>(), "Provides the list of parameters")
            ("threads,t", po::value<int>(), "Sets the number of threads")
//            ("no-skip", "Disables skipping of work that already has been finished")
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
        m_runtime->set_is_simulating(true);
        m_parameter_schema_path = vm["write-parameter-schema"].as<std::string>();
        if(!m_parameter_schema_path.parent_path().empty())
            boost::filesystem::create_directories(m_parameter_schema_path.parent_path());
    }
    if(vm.count("threads")) {
        if(!m_runtime->is_simulating()) {
            m_runtime->set_num_threads(vm["threads"].as<int>());
        }
        else {
            m_runtime->set_num_threads(1);
            std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This application will run only with 1 thread." << std::endl;
        }
    }
//    if(vm.count("no-skip")) {
//        if(!m_runtime->is_simulating()) {
//            m_runtime->enable_skipping = false;
//        }
//    }
    if(vm.count("parameters")) {
        std::cout << "<#> <#> Loading parameters from " << vm["parameters"].as<std::string>() << std::endl;
        std::ifstream in(vm["parameters"].as<std::string>());
        nlohmann::json j;
        in >> j;
        m_runtime->set_parameter_json(std::move(j));
    }
    else if(!m_runtime->is_simulating()) {
        throw std::runtime_error("You have to provide a parameter file!");
    }

    // Load runtime parameters that are not from CLI
    if(!vm.count("threads") && !m_runtime->is_simulating()) {
        m_runtime->set_num_threads(misaxx::parameter_registry:: template get_json<int>({ "runtime", "num-threads" }, misa_json_property<int>().with_default_value(1)));
    }
//            if(!vm.count("no-skip") && !m_runtime->is_simulating()) {
//                m_runtime->enable_skipping = !m_runtime->template get_json<bool>({ "runtime", "no-skip" }, false, misa_json_property<bool>());
//            }

    load_filesystem();

    return 0;
}

void misa_cli_base::run() {
    std::cout << "<#> <#> Starting run with " << m_runtime->get_num_threads() << " threads" << std::endl;
//            if(m_runtime.enable_skipping) {
//                std::cout << "<#> <#> Skipping is enabled" << std::endl;
//            }
//            else {
//                std::cout << "<#> <#> Skipping is disabled" << std::endl;
//            }
    if(m_runtime->is_simulating()) {
        std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This will build a parameter schema, but no real work is done!" << std::endl;
    }

    m_runtime->run();

    // Build schema
    if(m_runtime->is_simulating()) {
        std::cout << "<#> <#> Writing parameter schema to " << m_parameter_schema_path.string() << std::endl;
        m_runtime->get_schema_builder().write(m_parameter_schema_path);
    }
}

int misa_cli_base::prepare_and_run(const int argc, const char **argv) {
    int ret = prepare(argc, argv);
    if(ret != 0)
        return ret;
    run();
    return 0;
}

std::shared_ptr<misa_runtime_base> misa_cli_base::get_runtime() const {
    return m_runtime;
}
