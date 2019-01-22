#include <misaxx/core/runtime/misa_runtime_base.h>
#include <misaxx/core/utils/manual_stopwatch.h>
#include <omp.h>
#include <misaxx/core/workers/misa_work_subtree_status.h>
#include <misaxx/core/workers/misa_worker_status.h>
#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/attachments/misa_locatable_wrapper.h>
#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>

using namespace misaxx;

std::weak_ptr<misa_runtime_base> misa_runtime_base::singleton_instance = std::shared_ptr<misa_runtime_base>();

void misa_runtime_base::run_single_threaded() {

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
                nd->get_worker_status() == misa_worker_status::rejected) {
                if (nd->get_worker_status() == misa_worker_status::rejected) {
                    progress(*nd, "Retrying single-threaded work on");
                } else {
                    progress(*nd, "Starting single-threaded work on");
                }
                nd->work();
            }
            if (nd->get_worker_status() == misa_worker_status::rejected) {
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

        if (missing_dependency > 0) {
            progress("Info: " + std::to_string(missing_dependency) + " workers are waiting for dependencies");
        }
        if (rejected > 0) {
            progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
        }
    }
}

void misa_runtime_base::run_parallel() {

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
                nd->get_worker_status() == misa_worker_status::rejected) {
//                        if (!enable_skipping || !nd->is_skippable()) {
                if (!nd->is_parallelizeable()) {
                    if (nd->get_worker_status() == misa_worker_status::rejected) {
                        progress(*nd, "Retrying single-threaded work on");
                    } else {
                        progress(*nd, "Starting single-threaded work on");
                    }
                    nd->work();
                } else {
                    if (nd->get_worker_status() == misa_worker_status::rejected) {
                        progress(*nd, "Retrying parallelized work on");
                    } else {
                        progress(*nd, "Starting parallelized work on");
                    }

#pragma omp task shared(nd)
                    {
                        nd->work();
                    }
                }
//                        } else {
//                            progress(*nd, "Skipping");
//                            nd->skip_work();
//                        }
            }
            if (nd->get_worker_status() == misa_worker_status::rejected) {
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

        if (missing_dependency > 0) {
            progress("Info: " + std::to_string(missing_dependency) + " workers are waiting for dependencies");
        }
        if (rejected > 0) {
            progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
        }
    }
}

void misa_runtime_base::run() {

    misaxx::utils::manual_stopwatch stopwatch("Runtime");
    stopwatch.start();

    m_root = create_root_node();

    m_nodes_todo.push_back(m_root.get());
    m_nodes_todo_lookup.insert(m_root.get());
    ++m_known_nodes_count;

    const bool enable_threading = m_num_threads > 1 && !is_simulating();

    if(enable_threading)
        run_parallel();
    else
        run_single_threaded();

    // Postprocessing steps
    postprocess_caches();
    postprocess_cache_attachments();
    if(is_simulating()) {
        postprocess_parameter_schema();
    }
    if(!is_simulating()) {
        const auto parameters_path = get_filesystem().exported->external_path() / "parameters.json";
        const auto module_info_path = get_filesystem().exported->external_path() / "misa-module-info.json";

        // Write the parameter file
        std::cout << "<#> <#> Writing parameters to " << parameters_path << std::endl;
        std::ofstream writer;
        writer.open(parameters_path.string());
        writer << std::setw(4) << get_parameter_json();
        writer.close();

        // Write module info
        std::cout << "<#> <#> Writing module info to " << module_info_path << std::endl;
        writer.open(module_info_path.string());
        writer << std::setw(4) << nlohmann::json(get_module_info());
        writer.close();
    }
    if(!is_simulating()) {
        // Write the parameter schema
        m_is_simulating = true;

        // Clear everything
        m_nodes_todo.clear();
        m_nodes_todo_lookup.clear();
        m_known_nodes_count = 0;
        m_parameter_schema_builder = misa_json_schema_builder {};
        m_finished_nodes_count = 0;
        m_tree_complete = false;

        // To be safe, set the filesystem to something empty
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

        std::cout << "<#> <#> Writing parameter schema to " << output_path.string() << std::endl;
        get_schema_builder().write(output_path);

        m_is_simulating = false;
    }

    stopwatch.stop();
}

void misa_runtime_base::progress(const std::string &t_text) {
    std::stringstream stream;
    if (m_tree_complete) {
        stream << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
    } else {
        stream << "<#>";
    }
    stream << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";

    stream << "\t" << t_text;

    if (stream.str() != m_last_progress) {
        std::cout << stream.str() << std::endl;
        m_last_progress = stream.str();
    }
}

void misa_runtime_base::progress(const misa_work_node &t_node, const std::string &t_text) {
    std::stringstream stream;
    if (m_tree_complete) {
        stream << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
    } else {
        stream << "<#>";
    }
    stream << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";
    stream << "\t" << t_text << " " << *t_node.get_global_path();

    if (stream.str() != m_last_progress) {
        std::cout << stream.str() << std::endl;
        m_last_progress = stream.str();
    }
}

bool misa_runtime_base::is_simulating() const {
    return m_is_simulating;
}

void misa_runtime_base::register_cache(std::shared_ptr<misa_cache> t_cache) {
    m_registered_caches.emplace_back(std::move(t_cache));
}

const std::vector<std::shared_ptr<misa_cache>> &misa_runtime_base::get_registered_caches() const {
    return m_registered_caches;
}

nlohmann::json &misa_runtime_base::get_parameter_json() {
    return m_parameters;
}

nlohmann::json misa_runtime_base::get_json_raw(const std::vector<std::string> &t_path) {
    return misaxx::json_helper::access_json_path(m_parameters, t_path);
}

misa_json_schema_builder &misa_runtime_base::get_schema_builder() {
    return m_parameter_schema_builder;
}

std::shared_ptr<misa_work_node> misa_runtime_base::get_root_node() const {
    return m_root;
}

misa_runtime_base &misa_runtime_base::instance() {
    if (!singleton_instance.expired()) {
        return *singleton_instance.lock();
    } else {
        throw std::runtime_error("Runtime is not initialized!");
    }
}

bool misa_runtime_base::is_running() const {
    return !m_nodes_todo.empty();
}

void misa_runtime_base::set_is_simulating(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change simulation mode while the runtime is working!");
    m_is_simulating = value;
}

void misa_runtime_base::set_num_threads(int threads) {
    if (is_running())
        throw std::runtime_error("Cannot change the number of threads while the runtime is working!");
    if (threads < 1)
        throw std::runtime_error("Invalid number of threads!");
    m_num_threads = threads;
}

void misa_runtime_base::set_parameter_json(nlohmann::json t_json) {
    if (is_running())
        throw std::runtime_error("Cannot change the number of threads while the runtime is working!");
    m_parameters = std::move(t_json);
}

int misa_runtime_base::get_num_threads() const {
    return m_num_threads;
}

void misa_runtime_base::postprocess_caches() {
    if (!is_simulating()) {
        std::cout << "[Caches] Post-processing caches ..." << std::endl;
        const std::vector<std::shared_ptr<misa_cache>> &caches = get_registered_caches();
        for (const auto &ptr : caches) {
            std::cout << "[Caches] Post-processing cache " << ptr->get_location() << " (" << ptr->get_unique_location()
                      << ")" << std::endl;
            ptr->postprocess();
        }
    }
}

void misa_runtime_base::postprocess_cache_attachments() {
    std::cout << "[Attachments] Post-processing attachments ..." << std::endl;

    const std::vector<std::shared_ptr<misa_cache>> &caches = get_registered_caches();
    for (const auto &ptr : caches) {

        if (ptr->get_unique_location().empty())
            continue;

        std::cout << "[Attachments] Post-processing attachment " << ptr->get_location() << " ("
                  << ptr->get_unique_location() << ")" << std::endl;

        readonly_access<typename misa_cached_data_base::attachment_type> access(ptr->attachments); // Open the cache

        if (!is_simulating()) {
            boost::filesystem::path filesystem_unique_link_path = ptr->get_unique_location_in_filesystem();
            boost::filesystem::path filesystem_generic_link_path = ptr->get_location_in_filesystem();

            // Replace extension with JSON
            const boost::filesystem::path filesystem_export_base_path = get_filesystem().exported->external_path();
            boost::filesystem::path cache_attachment_path =
                    (filesystem_export_base_path / "attachments" / filesystem_unique_link_path).string() + ".json";
            boost::filesystem::create_directories(cache_attachment_path.parent_path());

            nlohmann::json exported_json = nlohmann::json(nlohmann::json::object());

            for (const auto &kv : access.get()) {
                const std::unique_ptr<misa_serializeable> &attachment_ptr = kv.second;

                if (!cache_attachment_path.empty()) {
                    // Export the attachment as JSON
                    attachment_ptr->to_json(exported_json[attachment_ptr->get_serialization_id().get_id()]);
                }
            }

            // Attach misa_filesystem_link if needed
            if (!access.get().has<misa_location>()) {
                misa_location link(filesystem_generic_link_path, filesystem_unique_link_path);
                link.to_json(exported_json[link.get_serialization_id().get_id()]);
            }

            // Attach the description storage if needed
            if(!access.get().has<misa_locatable_wrapper<misa_description_storage, misa_location>>()) {
                misa_location link(filesystem_generic_link_path, filesystem_unique_link_path);
                misa_description_storage descr(*ptr->describe());
                misa_locatable_wrapper<misa_description_storage, misa_location> attachment(std::move(descr), std::move(link));
                attachment.to_json(exported_json[attachment.get_serialization_id().get_id()]);
            }

            if (!is_simulating()) {
                std::ofstream sw;
                sw.open(cache_attachment_path.string());
                sw << std::setw(4) << exported_json;
            }
        }
    }
}

void misa_runtime_base::postprocess_parameter_schema() {
    misa_json_schema_builder &schema = get_schema_builder();

    // Save filesystem to parameter schema
    get_filesystem().to_json_schema(misa_json_schema(schema, {"filesystem", "json-data"}));
    schema.insert<std::string>({"filesystem", "source"},
                               misa_json_property<std::string>().with_default_value("json").make_required());

    // Workaround: Due to inflexibility with schema generation, manually put "__OBJECT__" nodes into list builders
    // /properties/algorithm -> nothing to do
    // /properties/objects/properties/__OBJECT__ -> /properties/objects/additionalProperties
    {
        auto &base = schema.data["properties"]["samples"];
        base["type"] = "object";
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
    schema.insert<int>({"runtime", "num-threads"},
                       misa_json_property<int>().with_title("Number of threads").with_default_value(1).make_optional());
//                schema.insert<bool>({"runtime", "no-skip"}, misa_json_property<bool>("Disable skipping", ""));

}
