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
#include <iomanip>
#include "misa_runtime.h"
#include "misa_readme_builder.h"

using namespace misaxx;

namespace misaxx {
    struct misa_cli_base_impl {
        std::shared_ptr<misa_runtime> m_runtime;
        boost::filesystem::path m_parameter_schema_path;
        boost::filesystem::path m_readme_path;
    };
}

misa_cli_base::misa_cli_base(std::shared_ptr<misa_runtime> t_runtime) : m_pimpl(new misa_cli_base_impl()) {
    m_pimpl->m_runtime = std::move(t_runtime);
}

misa_cli_base::~misa_cli_base() {
    delete m_pimpl;
}

void misa_cli_base::load_filesystem() {

    // Do no importing if we build a schema
    if(m_pimpl->m_runtime->is_simulating()) {
        misa_filesystem_empty_importer importer;
        m_pimpl->m_runtime->set_filesystem(importer.import());
        return;
    }

    // Otherwise load importer from parameters
    nlohmann::json &params = m_pimpl->m_runtime->get_parameter_json()["filesystem"];
    if(params["source"] == "directories") {
        misa_filesystem_directories_importer importer;
        importer.input_path = params["input-directory"].get<std::string>();
        importer.output_path = params["output-directory"].get<std::string>();
        m_pimpl->m_runtime->set_filesystem(importer.import());
    }
    else if(params["source"] == "json") {
        misa_filesystem_json_importer importer;
        if(params.find("json-data") != params.end()) {
            importer.input_json = params["json-data"];
        }
        else {
            importer.json_path = params["json-path"].get<std::string>();
        }
        m_pimpl->m_runtime->set_filesystem(importer.import());
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
            ("skip", "Requests that already existing results should be used instead of re-calculating them")
            ("write-parameter-schema", po::value<std::string>(), "Writes a parameter schema to the target file")
            ("write-readme", po::value<std::string>(), "Writes a README file to the target file")
            ("full-runtime-log", "Writes a comprehensive log containing the runtimes of each tasks into the output directory");

    po::command_line_parser parser(argc, argv);
    parser.options(general_options);
    po::parsed_options parsed_options = parser.run();

    po::variables_map vm;
    po::store(parsed_options, vm);
    po::notify(vm);

    if(vm.count("help")) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_id() << " " << info.get_version() << "\n";
        std::cout << general_options << "\n";
        return misa_cli_base::cli_result::no_workload;
    }
    if(vm.count("version")) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_id() << " " << info.get_version() << "\n";
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
        m_pimpl->m_runtime->set_is_simulating(true);
        m_pimpl->m_parameter_schema_path = vm["write-parameter-schema"].as<std::string>();
        if(!m_pimpl->m_parameter_schema_path.parent_path().empty())
            boost::filesystem::create_directories(m_pimpl->m_parameter_schema_path.parent_path());
    }
    if(vm.count("write-readme")) {
        m_pimpl->m_runtime->set_is_simulating(true);
        m_pimpl->m_readme_path = vm["write-readme"].as<std::string>();
        if(!m_pimpl->m_readme_path.parent_path().empty())
            boost::filesystem::create_directories(m_pimpl->m_readme_path.parent_path());
    }
    if(vm.count("skip")) {
        if(!m_pimpl->m_runtime->is_simulating()) {
            m_pimpl->m_runtime->set_request_skipping(true);
        }
    }
    if(vm.count("threads")) {
        if(!m_pimpl->m_runtime->is_simulating()) {
            m_pimpl->m_runtime->set_num_threads(vm["threads"].as<int>());
        }
        else {
            m_pimpl->m_runtime->set_num_threads(1);
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
        m_pimpl->m_runtime->set_parameter_json(std::move(j));
    }
    else if(!m_pimpl->m_runtime->is_simulating()) {
        throw std::runtime_error("You have to provide a parameter file!");
    }

    // Load runtime parameters that are not from CLI
    if(!vm.count("threads") && !m_pimpl->m_runtime->is_simulating()) {
        auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "num-threads" });
        schema->declare_optional<int>(1);
        m_pimpl->m_runtime->set_num_threads(misaxx::parameter_registry:: template get_json<int>({ "runtime", "num-threads" }));
    }
    if(!vm.count("skip") && !m_pimpl->m_runtime->requests_skipping()) {
        auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "request-skipping" });
        schema->declare_optional<bool>(false);
        m_pimpl->m_runtime->set_request_skipping(misaxx::parameter_registry:: template get_json<bool>({ "runtime", "request-skipping" }));
    }
    if(!m_pimpl->m_runtime->is_simulating()) {
        if(vm.count("full-runtime-log")) {
            m_pimpl->m_runtime->set_enable_runtime_log(true);
        }
        else {
            auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "full-runtime-log" });
            schema->declare_optional<bool>(false);
            m_pimpl->m_runtime->set_enable_runtime_log(misaxx::parameter_registry::get_json<bool>({ "runtime", "full-runtime-log" }));
        }
    }

//            if(!vm.count("no-skip") && !m_runtime->is_simulating()) {
//                m_runtime->enable_skipping = !m_runtime->template get_json<bool>({ "runtime", "no-skip" }, false, misa_json_property<bool>());
//            }

    load_filesystem();

    return misa_cli_base::cli_result::continue_with_workload;
}

void misa_cli_base::run() {
    std::cout << "<#> <#> Starting run with " << m_pimpl->m_runtime->get_num_threads() << " threads" << "\n";
//            if(m_runtime.enable_skipping) {
//                std::cout << "<#> <#> Skipping is enabled" << "\n";
//            }
//            else {
//                std::cout << "<#> <#> Skipping is disabled" << "\n";
//            }
    if(m_pimpl->m_runtime->is_simulating()) {
        std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This will build a parameter schema, but no real work is done!" << "\n";
    }

    m_pimpl->m_runtime->run();

    // Build schema
    if(m_pimpl->m_runtime->is_simulating()) {
        if(!m_pimpl->m_parameter_schema_path.empty()) {
            std::cout << "<#> <#> Writing parameter schema to " << m_pimpl->m_parameter_schema_path.string() << "\n";
            nlohmann::json j;
            m_pimpl->m_runtime->get_schema_builder()->to_json(j);
            std::ofstream w;
            w.open(m_pimpl->m_parameter_schema_path.string());
            w << std::setw(4) << j;
        }

        if(!m_pimpl->m_readme_path.empty()) {
            std::cout << "<#> <#> Writing README to " << m_pimpl->m_readme_path.string() << "\n";
            nlohmann::json j;
            m_pimpl->m_runtime->get_schema_builder()->to_json(j);
            build_readme(j, m_pimpl->m_readme_path);
        }
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
    return m_pimpl->m_runtime;
}



