#include <misaxx/runtime/misa_runtime_base.h>
#include <cxxh/measurement/manual_stopwatch.h>
#include <omp.h>
#include <misaxx/workers/task_tree/misa_work_subtree_status.h>
#include <misaxx/workers/task_tree/misa_worker_status.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include "misa_runtime_base.h"


using namespace misaxx;

std::weak_ptr<misa_runtime_base> misa_runtime_base::singleton_instance = std::shared_ptr<misa_runtime_base>();

void misa_runtime_base::run() {

    manual_stopwatch sw("Runtime");
    sw.start();

    m_root = create_root_node();

    m_nodes_todo.push_back(m_root.get());
    m_nodes_todo_lookup.insert(m_root.get());
    ++m_known_nodes_count;

    const bool enable_threading = m_num_threads > 1;

    if (enable_threading)
        omp_set_num_threads(this->m_num_threads);

    #pragma omp parallel if (enable_threading)
    #pragma omp single
    while (!m_nodes_todo.empty()) {

        size_t missing_dependency = 0;
        size_t rejected = 0;
        bool tree_complete = true;

        for (size_t i = 0; i < m_nodes_todo.size(); ++i) {
            auto *nd = m_nodes_todo[i];

            if (nd->get_subtree_status() != nodes::misa_work_subtree_status::complete)
                tree_complete = false;

            // First check if we even can do work
            if (!nd->dependencies_satisfied()) {
                ++missing_dependency;
                continue;
            }

            auto subtree_before = nd->get_subtree_status();
            if (nd->get_worker_status() == nodes::misa_worker_status::undone ||
                nd->get_worker_status() == nodes::misa_worker_status::rejected) {
//                        if (!enable_skipping || !nd->is_skippable()) {
                if (!enable_threading || !nd->is_parallelizeable()) {
                    if (nd->get_worker_status() == nodes::misa_worker_status::rejected) {
                        progress(*nd, "Retrying single-threaded work on");
                    } else {
                        progress(*nd, "Starting single-threaded work on");
                    }
                    nd->work();
                } else {
                    if (nd->get_worker_status() == nodes::misa_worker_status::rejected) {
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
            if (nd->get_worker_status() == nodes::misa_worker_status::rejected) {
                // If the work was rejected, don't do any additional steps afterwards
                ++rejected;
                continue;
            }
            if (subtree_before == nodes::misa_work_subtree_status::incomplete) {
                // Look for new nodes to visit
                for (auto &child : nd->get_children()) {
                    if (child->get_worker_status() == nodes::misa_worker_status::undone &&
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
            if (nd->get_worker_status() == nodes::misa_worker_status::done &&
                nd->get_subtree_status() == nodes::misa_work_subtree_status::complete) {
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

    sw.stop();
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

void misa_runtime_base::progress(const nodes::misa_work_node &t_node, const std::string &t_text) {
    std::stringstream stream;
    if (m_tree_complete) {
        stream << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
    } else {
        stream << "<#>";
    }
    stream << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";
    stream << "\t" << t_text << " " << t_node.get_json_path();

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

std::shared_ptr<nodes::misa_work_node> misa_runtime_base::get_root_node() const {
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
