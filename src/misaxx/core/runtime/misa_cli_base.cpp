//
// Created by rgerst on 14.12.18.
//

#include <boost/program_options.hpp>
#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>
#include <misaxx/core/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/core/filesystem/misa_filesystem_json_importer.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/utils/manual_stopwatch.h>
#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <misaxx/core/runtime/misa_cli_base.h>
#include "misa_runtime.h"

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

misa_cli_base::cli_result misa_cli_base::prepare(const int argc, const char **argv) {
    namespace po = boost::program_options;

    po::options_description general_options("Runtime options");
    general_options.add_options()
            ("help,h", "Help screen")
            ("version,v", "Prints the module name and version info")
            ("module-info", "Prints the module module information as serialized JSON")
            ("parameters,p", po::value<std::string>(), "Provides the list of parameters")
            ("threads,t", po::value<int>(), "Sets the number of threads")
            ("write-parameter-schema", po::value<std::string>(), "Writes a parameter schema to the target file")
            ("write-runtime-log", "Writes a log containing the runtimes of each tasks into the output directory");

    po::command_line_parser parser(argc, argv);
    parser.options(general_options);
    po::parsed_options parsed_options = parser.run();

    po::variables_map vm;
    po::store(parsed_options, vm);
    po::notify(vm);

    if(vm.count("help")) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_name() << " " << info.get_version() << "\n";
        std::cout << general_options << "\n";
        return misa_cli_base::cli_result::no_workload;
    }
    if(vm.count("version")) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_name() << " " << info.get_version() << "\n";
        return misa_cli_base::cli_result::no_workload;
    }
    if(vm.count("module-info")) {
        auto info = misaxx::runtime_properties::get_module_info();
        nlohmann::json json;
        info.to_json(json);
        std::cout << json << "\n";
        return misa_cli_base::cli_result::no_workload;
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
            std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This application will run only with 1 thread." << "\n";
        }
    }
//    if(vm.count("no-skip")) {
//        if(!m_runtime->is_simulating()) {
//            m_runtime->enable_skipping = false;
//        }
//    }
    if(vm.count("parameters")) {
        std::cout << "<#> <#> Loading parameters from " << vm["parameters"].as<std::string>() << "\n";
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
    if(vm.count("write-runtime-log")) {
        m_runtime->set_enable_runtime_log(true);
    }
    else {
        m_runtime->set_enable_runtime_log(misaxx::parameter_registry::get_json<bool>({
                                                                                             "runtime", "write-runtime-log"
                                                                                     }, misaxx::misa_json_property<bool>().with_default_value(false)));
    }
//            if(!vm.count("no-skip") && !m_runtime->is_simulating()) {
//                m_runtime->enable_skipping = !m_runtime->template get_json<bool>({ "runtime", "no-skip" }, false, misa_json_property<bool>());
//            }

    load_filesystem();

    return misa_cli_base::cli_result::continue_with_workload;
}

void misa_cli_base::run() {
    std::cout << "<#> <#> Starting run with " << m_runtime->get_num_threads() << " threads" << "\n";
//            if(m_runtime.enable_skipping) {
//                std::cout << "<#> <#> Skipping is enabled" << "\n";
//            }
//            else {
//                std::cout << "<#> <#> Skipping is disabled" << "\n";
//            }
    if(m_runtime->is_simulating()) {
        std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This will build a parameter schema, but no real work is done!" << "\n";
    }

    m_runtime->run();

    // Build schema
    if(m_runtime->is_simulating()) {
        std::cout << "<#> <#> Writing parameter schema to " << m_parameter_schema_path.string() << "\n";
        m_runtime->get_schema_builder().write(m_parameter_schema_path);
    }
}

int misa_cli_base::prepare_and_run(const int argc, const char **argv) {
    const misa_cli_base::cli_result ret = prepare(argc, argv);
    switch(ret) {
        case misa_cli_base::cli_result::continue_with_workload:
            run();
            return 0;
        case misa_cli_base::cli_result::no_workload:
            return 0;
        case misa_cli_base::cli_result::error:
            return 1;
        default:
            throw std::runtime_error("Unsupported cli result case!");
    }
}

std::shared_ptr<misa_runtime> misa_cli_base::get_runtime() const {
    return m_runtime;
}
