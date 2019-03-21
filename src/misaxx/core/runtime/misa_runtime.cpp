#include <src/misaxx/core/runtime/misa_runtime.h>
#include <misaxx/core/utils/manual_stopwatch.h>
#include <omp.h>
#include <misaxx/core/workers/misa_work_subtree_status.h>
#include <misaxx/core/workers/misa_worker_status.h>
#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>
#include <misaxx/core/utils/string.h>
#include <misaxx/core/misa_json_schema_property.h>
#include <iomanip>
#include <iostream>
#include "misa_runtime.h"


using namespace misaxx;

std::weak_ptr<misa_runtime> misa_runtime::singleton_instance = std::shared_ptr<misa_runtime>();

void misa_runtime::run_single_threaded() {

    while (!m_nodes_todo.empty()) {

        size_t missing_dependency = 0;
        size_t rejected = 0;
        bool tree_complete = true;

        for (size_t i = 0; i < m_nodes_todo.size(); ++i) {
            auto *nd = m_nodes_todo[i];

            if (nd->get_subtree_status() != misa_work_subtree_status::complete)
                tree_complete = false;

            // First check if we even can do work
            if (!nd->dependencies_satisfied()) {
                ++missing_dependency;
                continue;
            }

            auto subtree_before = nd->get_subtree_status();
            if (nd->get_worker_status() == misa_worker_status::undone ||
                nd->get_worker_status() == misa_worker_status::queued_repeat) {
                if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                    progress(*nd, "Retrying single-threaded work on");
                } else {
                    progress(*nd, "Starting single-threaded work on");
                }
                if (m_enable_runtime_log) {
                    m_runtime_log.start(0, misaxx::utils::to_string(*nd->get_global_path()));
                }
                nd->prepare_work();
                nd->work();
                if (m_enable_runtime_log) {
                    m_runtime_log.stop(0);
                }
            }
            if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                // If the work was rejected, don't do any additional steps afterwards
                ++rejected;
                continue;
            }
            if (subtree_before == misa_work_subtree_status::incomplete) {
                // Look for new nodes to visit
                for (auto &child : nd->get_children()) {
                    if (child->get_worker_status() == misa_worker_status::undone &&
                        m_nodes_todo_lookup.find(child.get()) == m_nodes_todo_lookup.end()) {
                        m_nodes_todo_lookup.insert(child.get());
                        m_nodes_todo.push_back(child.get());
                        ++m_known_nodes_count;

//                                if(build_schema) {
//                                    parameter_schema.insert_node(child->get_path(), child->get_metadata());
//                                }
                    }
                }
            }
            if (nd->get_worker_status() == misa_worker_status::done &&
                nd->get_subtree_status() == misa_work_subtree_status::complete) {
                // Use quick-delete to remove the completed node
                std::swap(m_nodes_todo[i], m_nodes_todo[m_nodes_todo.size() - 1]);
                m_nodes_todo.resize(m_nodes_todo.size() - 1);
                --i;
                ++m_finished_nodes_count;
                progress(*nd, "Work finished on");
            }
        }

        m_tree_complete |= tree_complete;

        if (missing_dependency > 0 && missing_dependency != m_last_waiting_announcement) {
            progress("Info: " + std::to_string(missing_dependency) + " workers are waiting for dependencies");
            m_last_waiting_announcement = missing_dependency;
        }
        if (rejected > 0 && rejected != m_last_rejecting_announcement) {
            progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
            m_last_rejecting_announcement = rejected;
        }
    }
}

void misa_runtime::run_parallel() {

    omp_set_num_threads(this->m_num_threads);

#pragma omp parallel
#pragma omp single
    while (!m_nodes_todo.empty()) {

        size_t missing_dependency = 0;
        size_t rejected = 0;
        bool tree_complete = true;

        for (size_t i = 0; i < m_nodes_todo.size(); ++i) {
            auto *nd = m_nodes_todo[i];

            if (nd->get_subtree_status() != misa_work_subtree_status::complete)
                tree_complete = false;

            // First check if we even can do work
            if (!nd->dependencies_satisfied()) {
                ++missing_dependency;
                continue;
            }

            auto subtree_before = nd->get_subtree_status();
            if (nd->get_worker_status() == misa_worker_status::undone ||
                nd->get_worker_status() == misa_worker_status::queued_repeat) {
//                        if (!enable_skipping || !nd->is_skippable()) {
                if (!nd->is_parallelizeable()) {
                    if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                        progress(*nd, "Retrying single-threaded work on");
                    } else {
                        progress(*nd, "Starting single-threaded work on");
                    }
                    if (m_enable_runtime_log) {
                        m_runtime_log.start(0, misaxx::utils::to_string(*nd->get_global_path()));
                    }
                    nd->prepare_work();
                    nd->work();
                    if (m_enable_runtime_log) {
                        m_runtime_log.stop(0);
                    }
                } else {
                    if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                        progress(*nd, "Retrying parallelized work on");
                    } else {
                        progress(*nd, "Starting parallelized work on");
                    }

                    nd->prepare_work();
#pragma omp task shared(nd)
                    {
                        if (m_enable_runtime_log) {
                            m_runtime_log.start(omp_get_thread_num(), misaxx::utils::to_string(*nd->get_global_path()));
                        }
                        nd->work();
                        if (m_enable_runtime_log) {
                            m_runtime_log.stop(omp_get_thread_num());
                        }
                    }
                }
//                        } else {
//                            progress(*nd, "Skipping");
//                            nd->skip_work();
//                        }
            }
            if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                // If the work was rejected, don't do any additional steps afterwards
                ++rejected;
                continue;
            }
            if (subtree_before == misa_work_subtree_status::incomplete) {
                // Look for new nodes to visit
                for (auto &child : nd->get_children()) {
                    if (child->get_worker_status() == misa_worker_status::undone &&
                        m_nodes_todo_lookup.find(child.get()) == m_nodes_todo_lookup.end()) {
                        m_nodes_todo_lookup.insert(child.get());
                        m_nodes_todo.push_back(child.get());
                        ++m_known_nodes_count;

//                                if(build_schema) {
//                                    parameter_schema.insert_node(child->get_path(), child->get_metadata());
//                                }
                    }
                }
            }
            if (nd->get_worker_status() == misa_worker_status::done &&
                nd->get_subtree_status() == misa_work_subtree_status::complete) {
                // Use quick-delete to remove the completed node
                std::swap(m_nodes_todo[i], m_nodes_todo[m_nodes_todo.size() - 1]);
                m_nodes_todo.resize(m_nodes_todo.size() - 1);
                --i;
                ++m_finished_nodes_count;
                progress(*nd, "Work finished on");
            }
        }

        m_tree_complete |= tree_complete;

        if (missing_dependency > 0 && missing_dependency != m_last_waiting_announcement) {
            progress("Info: " + std::to_string(missing_dependency) + " workers are waiting for dependencies");
            m_last_waiting_announcement = missing_dependency;
        }
        if (rejected > 0 && rejected != m_last_rejecting_announcement) {
            progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
            m_last_rejecting_announcement = rejected;
        }
    }
}

void misa_runtime::run() {

    misaxx::utils::manual_stopwatch stopwatch("Runtime");
    stopwatch.start();

    m_root = create_root_node();

    m_nodes_todo.push_back(m_root.get());
    m_nodes_todo_lookup.insert(m_root.get());
    ++m_known_nodes_count;

    const bool enable_threading = m_num_threads > 1 && !is_simulating();

    if (!m_enable_runtime_log) {
        for (int thread = 0; thread < m_num_threads; ++thread) {
            m_runtime_log.start(thread, "Undefined workload");
        }
    }

    if (enable_threading)
        run_parallel();
    else
        run_single_threaded();

    if (!m_enable_runtime_log) {
        for (int thread = 0; thread < m_num_threads; ++thread) {
            m_runtime_log.stop(thread);
        }
    }

    // Postprocessing steps
    stopwatch.new_operation("Postprocessing");
    postprocess_caches();
    postprocess_cache_attachments();
    if (is_simulating()) {
        postprocess_parameter_schema();
    }
    if (!is_simulating()) {
        if (!boost::filesystem::exists(get_filesystem().exported->external_path())) {
            boost::filesystem::create_directories(get_filesystem().exported->external_path());
        }

        const auto parameters_path = get_filesystem().exported->external_path() / "parameters.json";
        const auto module_info_path = get_filesystem().exported->external_path() / "misa-module-info.json";

        // Write the parameter file
        std::cout << "<#> <#> Writing parameters to " << parameters_path << "\n";
        std::ofstream writer;
        writer.open(parameters_path.string());
        writer << std::setw(4) << get_parameter_json();
        writer.close();

        // Write module info
        std::cout << "<#> <#> Writing module info to " << module_info_path << "\n";
        writer.open(module_info_path.string());
        writer << std::setw(4) << nlohmann::json(get_module_info());
        writer.close();
    }
    if (!is_simulating()) {
        // Write the parameter schema
        m_is_simulating = true;

        // Clear everything
        m_nodes_todo.clear();
        m_nodes_todo_lookup.clear();
        m_known_nodes_count = 0;
        m_parameter_schema_builder = std::make_shared<misa_json_schema_property>();
        m_finished_nodes_count = 0;
        m_tree_complete = false;

        // To be safe, set the filesystem to something empty
        const auto runtime_log_output_path = get_filesystem().exported->external_path() / "runtime-log.json";
        const auto output_path = get_filesystem().exported->external_path() / "parameter-schema.json";
        misa_filesystem_empty_importer importer;
        set_filesystem(importer.import());

        // Instantiate root node
        m_root = create_root_node();

        m_nodes_todo.push_back(m_root.get());
        m_nodes_todo_lookup.insert(m_root.get());
        ++m_known_nodes_count;

        // Run the parameter schema workload
        run_single_threaded();

        postprocess_parameter_schema();

        {
            std::cout << "<#> <#> Writing parameter schema to " << output_path.string() << "\n";
            nlohmann::json j;
            get_schema_builder()->to_json(j);
            std::ofstream out;
            out.open(output_path.string());
            out << std::setw(4) << j;
            out.close();
        }

        m_is_simulating = false;

        // Write the runtime log
        {
            std::cout << "<#> <#> Writing runtime log to " << runtime_log_output_path.string() << "\n";
            nlohmann::json j;
            m_runtime_log.to_json(j);
            std::ofstream out;
            out.open(runtime_log_output_path.string());
            out << std::setw(4) << j;
            out.close();
        }
    }

    stopwatch.stop();
}

void misa_runtime::progress(const std::string &t_text) {
    if (m_tree_complete) {
        std::cout << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
    } else {
        std::cout << "<#>";
    }
    std::cout << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";

    std::cout << "\t" << t_text;
    std::cout << "\n";
}

void misa_runtime::progress(const misa_work_node &t_node, const std::string &t_text) {
    if (m_tree_complete) {
        std::cout << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
    } else {
        std::cout << "<#>";
    }
    std::cout << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";
    std::cout << "\t" << t_text << " " << *t_node.get_global_path();
    std::cout << "\n";
}

bool misa_runtime::is_simulating() const {
    return m_is_simulating;
}

void misa_runtime::register_cache(std::shared_ptr<misa_cache> t_cache) {
    m_registered_caches.insert(std::move(t_cache));
}

bool misa_runtime::unregister_cache(const std::shared_ptr<misa_cache> &t_cache) {
    if (m_registered_caches.count(t_cache) > 0) {
        m_registered_caches.erase(t_cache);
        return true;
    }
    return false;
}

const std::unordered_set<std::shared_ptr<misa_cache>> &misa_runtime::get_registered_caches() const {
    return m_registered_caches;
}

nlohmann::json &misa_runtime::get_parameter_json() {
    return m_parameters;
}

nlohmann::json misa_runtime::get_json_raw(const std::vector<std::string> &t_path) {
    nlohmann::json *result = &m_parameters;
    for(const auto &key : t_path) {
        result = &((*result)[key]);
    }
    return *result;
}

std::shared_ptr<misa_json_schema_property> misa_runtime::get_schema_builder() {
    return m_parameter_schema_builder;
}

std::shared_ptr<misa_work_node> misa_runtime::get_root_node() const {
    return m_root;
}

misa_runtime &misa_runtime::instance() {
    if (!singleton_instance.expired()) {
        return *singleton_instance.lock();
    } else {
        throw std::runtime_error("Runtime is not initialized!");
    }
}

bool misa_runtime::is_running() const {
    return !m_nodes_todo.empty();
}

void misa_runtime::set_is_simulating(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change simulation mode while the runtime is working!");
    m_is_simulating = value;
}

void misa_runtime::set_num_threads(int threads) {
    if (is_running())
        throw std::runtime_error("Cannot change the number of threads while the runtime is working!");
    if (threads < 1)
        throw std::runtime_error("Invalid number of threads!");
    m_num_threads = threads;
}

void misa_runtime::set_parameter_json(nlohmann::json t_json) {
    if (is_running())
        throw std::runtime_error("Cannot change the number of threads while the runtime is working!");
    m_parameters = std::move(t_json);
}

int misa_runtime::get_num_threads() const {
    return m_num_threads;
}

void misa_runtime::postprocess_caches() {
    if (!is_simulating()) {
        std::cout << "[Caches] Post-processing caches ..." << "\n";
        if (!m_enable_runtime_log) {
            m_runtime_log.start(0, "Postprocessing");
        }
        for (const auto &ptr : get_registered_caches()) {
            if (m_enable_runtime_log) {
                m_runtime_log.start(0, "Postprocessing " + ptr->get_location().string() + " (" +
                                       ptr->get_unique_location().string() + ")");
            }
            std::cout << "[Caches] Post-processing cache " << ptr->get_location() << " (" << ptr->get_unique_location()
                      << ")" << "\n";
            ptr->postprocess();
            if (ptr->has_data()) {
                std::cout << "[Caches] Info: " << ptr->get_location() << " (" << ptr->get_unique_location()
                          << ")" << " reports that it still contains data" << "\n";
            }
            if (m_enable_runtime_log) {
                m_runtime_log.stop(0);
            }
        }
        if (!m_enable_runtime_log) {
            m_runtime_log.stop(0);
        }
    }
}

void misa_runtime::postprocess_cache_attachments() {

    if (!m_write_attachments) {
        std::cout << "[Attachments] Post-processing attachments ... Skipped" << "\n";
        return;
    }

    std::cout << "[Attachments] Post-processing attachments ..." << "\n";

    if (!m_enable_runtime_log) {
        m_runtime_log.start(0, "Attachments");
    }

    // Attachment JSON schemata
    nlohmann::json attachment_schemata;

    for (const auto &ptr : get_registered_caches()) {

        if (ptr->get_unique_location().empty())
            continue;

        if (!is_simulating()) {

            if (m_enable_runtime_log) {
                m_runtime_log.start(0, "Attachments: " + ptr->get_location().string() + " (" +
                                       ptr->get_unique_location().string() + ")");
            }

            readonly_access<typename misa_cached_data_base::attachment_type> access(ptr->attachments); // Open the cache

            if (m_lazy_write_attachments && access.get().empty()) {
                continue;
            }

            std::cout << "[Attachments] Post-processing attachment " << ptr->get_location() << " ("
                      << ptr->get_unique_location() << ")" << "\n";

            boost::filesystem::path filesystem_unique_link_path = ptr->get_internal_unique_location();
            boost::filesystem::path filesystem_generic_link_path = ptr->get_internal_location();

            // Replace extension with JSON
            const boost::filesystem::path filesystem_export_base_path = get_filesystem().exported->external_path();
            boost::filesystem::path cache_attachment_path =
                    (filesystem_export_base_path / "attachments" / filesystem_unique_link_path).string() + ".json";
            boost::filesystem::create_directories(cache_attachment_path.parent_path());

            nlohmann::json exported_json = nlohmann::json(nlohmann::json::object());

            for (const auto &kv : access.get()) {
                const std::unique_ptr<misa_serializable> &attachment_ptr = kv.second;

                if (!cache_attachment_path.empty()) {
                    // Export the attachment as JSON
                    attachment_ptr->to_json(exported_json[attachment_ptr->get_serialization_id().get_id()]);

                    // Export attachment JSON schema
                    if (attachment_schemata.find(attachment_ptr->get_serialization_id().get_id()) ==
                        attachment_schemata.end()) {
                        auto schema = std::make_shared<misa_json_schema_property>();
                        attachment_ptr->to_json_schema(*schema);
                        schema->to_json(attachment_schemata[attachment_ptr->get_serialization_id().get_id()]);
                    }
                }
            }

            // Attach the location
            ptr->get_location_interface()->to_json(exported_json["location"]);

            // Add schema for location type if needed
            if(attachment_schemata.find( ptr->get_location_interface()->get_serialization_id().get_id()) ==
               attachment_schemata.end()) {
                auto schema = std::make_shared<misa_json_schema_property>();
                ptr->get_location_interface()->to_json_schema(*schema);
                schema->to_json(attachment_schemata[ptr->get_location_interface()->get_serialization_id().get_id()]);
            }

            // Attach the description storage if needed
            if (!access.get().has<misa_description_storage>()) {
                misa_location link(ptr->get_internal_location(), filesystem_generic_link_path, filesystem_unique_link_path);
                ptr->describe()->set_location(ptr->get_location_interface());
                ptr->describe()->to_json(exported_json[ptr->describe()->get_serialization_id().get_id()]);

                // Add schema for description storage if needed
                if(attachment_schemata.find( ptr->describe()->get_serialization_id().get_id()) ==
                   attachment_schemata.end()) {
                    auto schema = std::make_shared<misa_json_schema_property>();
                    ptr->describe()->to_json_schema(*schema);
                    schema->to_json(attachment_schemata[ptr->describe()->get_serialization_id().get_id()]);
                }
            }

            // Add schemata for pattern & description
            if(ptr->describe()->has_pattern()) {
                const misa_data_pattern &descr = ptr->describe()->get<misa_data_pattern>();
                if(attachment_schemata.find( descr.get_serialization_id().get_id()) ==
                   attachment_schemata.end()) {
                    auto schema = std::make_shared<misa_json_schema_property>();
                    descr.to_json_schema(*schema);
                    schema->to_json(attachment_schemata[descr.get_serialization_id().get_id()]);
                }
            }
            if(ptr->describe()->has_description()) {
                const misa_data_description &descr = ptr->describe()->get<misa_data_description>();
                if(attachment_schemata.find( descr.get_serialization_id().get_id()) ==
                   attachment_schemata.end()) {
                    auto schema = std::make_shared<misa_json_schema_property>();
                    descr.to_json_schema(*schema);
                    schema->to_json(attachment_schemata[descr.get_serialization_id().get_id()]);
                }
            }

            if (!is_simulating()) {
                std::ofstream sw;
                sw.open(cache_attachment_path.string());
                sw << std::setw(4) << exported_json;
            }

            if (m_enable_runtime_log) {
                m_runtime_log.stop(0);
            }
        }
    }

    // Write attachment serialization IDs
    if (!is_simulating()) {
        const boost::filesystem::path filesystem_export_base_path = get_filesystem().exported->external_path();
        std::ofstream sw;
        sw.open((filesystem_export_base_path / "attachments" / "serialization-schemas.json").string());
        sw << std::setw(4) << attachment_schemata;
    }

    if (!m_enable_runtime_log) {
        m_runtime_log.stop(0);
    }
}

void misa_runtime::postprocess_parameter_schema() {
    std::shared_ptr<misa_json_schema_property> schema = get_schema_builder();

    // Save filesystem to parameter schema
    get_filesystem().to_json_schema(*(schema->resolve("filesystem")->resolve("json-data")));
    (*schema)["filesystem"]["source"].define<std::string>("json");

    // Workaround: Due to inflexibility with schema generation, manually put "__OBJECT__" nodes into list builders
    // /properties/algorithm -> nothing to do
    // /properties/objects/properties/__OBJECT__ -> /properties/objects/additionalProperties
    (*schema)["samples"]["__OBJECT__"]; // Needed if there are no sample parameters
    (*schema)["samples"].make_template();

    // /properties/runtime::filesystem/properties/json-data/properties/imported/properties/children/properties/__OBJECT__ -> /properties/runtime::filesystem/properties/json-data/properties/imported/properties/children/additionalProperties
    (*schema)["filesystem"]["json-data"]["imported"]["children"]["__OBJECT__"];
    (*schema)["filesystem"]["json-data"]["imported"]["children"].make_template();
    (*schema)["filesystem"]["json-data"]["exported"]["children"]["__OBJECT__"];
    (*schema)["filesystem"]["json-data"]["exported"]["children"].make_template();

    // Write runtime parameters
    (*schema)["runtime"]["num-threads"].document_title("Number of threads")
            .document_description("Changes the number of threads")
            .declare_optional<int>(1);
    (*schema)["runtime"]["full-runtime-log"].document_title("Full runtime log")
            .document_description("If enabled, the runtime log will contain all individual workers")
            .declare_optional(false);
    (*schema)["runtime"]["postprocessing"]["write-attachments"].document_title("Write attachments")
            .document_description("If enabled, write data attached to caches after the work is done")
            .declare_optional(true);
    (*schema)["runtime"]["postprocessing"]["lazy-write-attachments"].document_title("Only write attachments with actual data")
            .document_description("If enabled, only non-empty attachment files will be written")
            .declare_optional(true);
}

misa_runtime::misa_runtime(misa_module_info info, std::shared_ptr<misa_module_interface> interface,
                           std::function<std::shared_ptr<misa_dispatcher>(const std::shared_ptr<misa_work_node> &,
                                                                          std::shared_ptr<misa_module_interface>)> root_instantiator)
        : m_module_info(std::move(info)),
          m_module_interface(std::move(interface)),
          m_module_instantiator(std::move(root_instantiator)),
          m_parameter_schema_builder(std::make_shared<misa_json_schema_property>()) {

}

misa_filesystem misa_runtime::get_filesystem() {
    return m_module_interface->filesystem;
}

void misa_runtime::set_filesystem(misa_filesystem t_filesystem) {
    m_module_interface->filesystem = std::move(t_filesystem);
}

misa_module_info misa_runtime::get_module_info() {
    return m_module_info;
}

std::shared_ptr<misa_work_node> misa_runtime::create_root_node() {
//    auto result = m_module_instantiator(m_module_interface);
//    m_module_interface = result->get_module();
//    return result;
    return misa_work_node::create_instance(m_module_info.get_id(), std::shared_ptr<misa_work_node>(),
                                           [this](const std::shared_ptr<misa_work_node> &t_root) {
                                               auto root_module = this->m_module_instantiator(t_root, std::move(
                                                       this->m_module_interface));
                                               this->m_module_interface = root_module->get_module();
                                               return root_module;
                                           });
}

void misa_runtime::set_lazy_write_attachments(bool value) {
    m_lazy_write_attachments = value;
}

void misa_runtime::set_write_attachments(bool value) {
    m_write_attachments = value;
}

void misa_runtime::set_enable_runtime_log(bool value) {
    m_enable_runtime_log = value;
}

misa_runtime_log &misa_runtime::get_runtime_log() {
    return m_runtime_log;
}

bool misa_runtime::requests_skipping() const {
    return m_requests_skipping;
}

void misa_runtime::set_request_skipping(bool value) {
    m_requests_skipping = value;
}

