//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/runtime/misa_runtime_base.h>
#include <misaxx/json/misa_json_schema_builder.h>
#include <cxxh/measurement/manual_stopwatch.h>
#include <misaxx/workers/task_tree/misa_work_node.h>
#include <misaxx/misa_root_module_base.h>
#include <omp.h>
#include <misaxx/json/misa_json_helper.h>

namespace misaxx {

    template<class RootModule>
    class misa_runtime : public misa_runtime_base {

        static_assert(std::is_base_of<misa_root_module_base, RootModule>::value, "Only root modules can be instantiated!");

    public:

        /**
         * Module declaration type of the root node
         */
        using module_declaration_type = typename RootModule::module_declaration_type;

        /**
         * If true, the runtime can skip workers
         */
        bool enable_skipping = true;

        /**
         * Number of threads used by the runtime.
         * If the number is 1, the application will run in the current thread.
         */
        int num_threads = 1;

        /**
         * Parameters for the workers
         */
        nlohmann::json parameters;

        /**
         * Used to create a configuration schema
         */
        misa_json_schema_builder parameter_schema;

        /**
         * Name of the root module
         */
        std::string m_root_dispatcher_name;

        /**
         * Instantiates the default runtime and provides the root instantiator with necessary parameters
         * @tparam Args
         * @param args
         */
        explicit misa_runtime(std::string t_name) : m_root_dispatcher_name(std::move(t_name)) {

        }

        void set_is_simulating(bool value) {
            m_is_simulating = value;
        }

        misa_json_schema_builder& get_schema_builder() override {
           return parameter_schema;
        }

        misa_filesystem get_filesystem() override {
            return instance().filesystem;
        }

        nlohmann::json &get_parameter_json() override {
            return parameters;
        }

        nlohmann::json get_json_raw(const std::vector<std::string> &t_path) override {
            return misaxx::json_helper::access_json_path(parameters, t_path);
        }

        /**
         * Returns the instance of the root dispatcher
         * @return
         */
        RootModule &instance() {
            return dynamic_cast<RootModule&>(*m_root->get_instance());
        }

        void set_root_filesystem(misa_filesystem t_filesystem) {
            m_root_declaration.filesystem = std::move(t_filesystem);
        }

        void register_cache(std::shared_ptr<misa_cache> t_cache) override {
            m_registered_caches.emplace_back(std::move(t_cache));
        }

        /**
        * Returns the list of registered caches
        * @return
        */
        const std::vector<std::shared_ptr<misa_cache>> &get_registered_caches() const {
            return m_registered_caches;
        }

        /**
         * Starts the actual work of the runtime.
         */
        void run() {

            manual_stopwatch sw("Runtime");
            sw.start();

            // Instantiate the root here
            m_root = std::make_shared<nodes::misa_work_node>(m_root_dispatcher_name,
                    std::shared_ptr<nodes::misa_work_node>(), [this](const std::shared_ptr<nodes::misa_work_node> &t_root) {
                auto root_module = std::make_shared<RootModule>(t_root, std::move(m_root_declaration));
                return root_module;
            });
//            if(build_schema) {
//                parameter_schema.insert_node(m_root->get_path(), m_root->get_metadata());
//            }

            m_nodes_todo.push_back(m_root.get());
            m_nodes_todo_lookup.insert(m_root.get());
            ++m_known_nodes_count;

            const bool enable_threading = num_threads > 1;

            if(enable_threading)
                omp_set_num_threads(this->num_threads);

#pragma omp parallel if (enable_threading)
#pragma omp single
            while (!m_nodes_todo.empty()) {

                size_t missing_dependency = 0;
                size_t rejected = 0;
                bool tree_complete = true;

                for (size_t i = 0; i < m_nodes_todo.size(); ++i) {
                    auto *nd = m_nodes_todo[i];

                    if(nd->get_subtree_status() != nodes::misa_work_subtree_status::complete)
                        tree_complete = false;

                    // First check if we even can do work
                    if(!nd->dependencies_satisfied()) {
                        ++missing_dependency;
                        continue;
                    }

                    auto subtree_before = nd->get_subtree_status();
                    if (nd->get_worker_status() == nodes::misa_worker_status::undone ||
                        nd->get_worker_status() == nodes::misa_worker_status::rejected) {
//                        if (!enable_skipping || !nd->is_skippable()) {
                            if(!enable_threading || !nd->is_parallelizeable()) {
                                if(nd->get_worker_status() == nodes::misa_worker_status::rejected) {
                                    progress(*nd, "Retrying single-threaded work on");
                                }
                                else {
                                    progress(*nd, "Starting single-threaded work on");
                                }
                                nd->work();
                            }
                            else {
                                if(nd->get_worker_status() == nodes::misa_worker_status::rejected) {
                                    progress(*nd, "Retrying parallelized work on");
                                }
                                else {
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
                    if(nd->get_worker_status() == nodes::misa_worker_status::rejected) {
                        // If the work was rejected, don't do any additional steps afterwards
                        ++rejected;
                        continue;
                    }
                    if (subtree_before == nodes::misa_work_subtree_status::incomplete) {
                        // Look for new nodes to visit
                        for (auto &child : nd->get_children()) {
                            if (child->get_worker_status() == nodes::misa_worker_status::undone && m_nodes_todo_lookup.find(child.get()) == m_nodes_todo_lookup.end()) {
                                m_nodes_todo_lookup.insert(child.get());
                                m_nodes_todo.push_back(child.get());
                                ++m_known_nodes_count;

//                                if(build_schema) {
//                                    parameter_schema.insert_node(child->get_path(), child->get_metadata());
//                                }
                            }
                        }
                    }
                    if (nd->get_worker_status() == nodes::misa_worker_status::done && nd->get_subtree_status() == nodes::misa_work_subtree_status::complete) {
                        // Use quick-delete to remove the completed node
                        std::swap(m_nodes_todo[i], m_nodes_todo[m_nodes_todo.size() - 1]);
                        m_nodes_todo.resize(m_nodes_todo.size() - 1);
                        --i;
                        ++m_finished_nodes_count;
                        progress(*nd, "Work finished on");
                    }
                }

                m_tree_complete |= tree_complete;

                if(missing_dependency > 0)  {
                    progress("Info: " + std::to_string(missing_dependency) + " workers are waiting for dependencies");
                }
                if(rejected > 0)  {
                    progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
                }
            }

            sw.stop();
        }

    private:

        void progress(const std::string &t_text) {
            std::stringstream stream;
            if(m_tree_complete) {
                stream << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
            }
            else {
                stream << "<#>";
            }
            stream << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";

            stream << "\t" << t_text;

            if(stream.str() != m_last_progress) {
                std::cout << stream.str() << std::endl;
                m_last_progress = stream.str();
            }
        }

        void progress(const nodes::misa_work_node &t_node, const std::string &t_text) {
            std::stringstream stream;
            if(m_tree_complete) {
                stream << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
            }
            else {
                stream << "<#>";
            }
            stream << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";
            stream << "\t" << t_text << " " << t_node.get_json_path();

            if(stream.str() != m_last_progress) {
                std::cout << stream.str() << std::endl;
                m_last_progress = stream.str();
            }
        }

    private:

        std::shared_ptr<nodes::misa_work_node> m_root;

        std::vector<std::shared_ptr<misa_cache>> m_registered_caches;

        size_t m_known_nodes_count = 0;

        size_t m_finished_nodes_count = 0;

        bool m_tree_complete = false;

        std::string m_last_progress;

        /**
         * List of nodes that are not completed and need to be watched
         */
        std::vector<nodes::misa_work_node *> m_nodes_todo;
        /**
         * List of nodes for fast lookup of unknown nodes
         */
        std::unordered_set<nodes::misa_work_node *> m_nodes_todo_lookup;

        /**
        * Root filesystem
        */
        module_declaration_type m_root_declaration;

    };
}
