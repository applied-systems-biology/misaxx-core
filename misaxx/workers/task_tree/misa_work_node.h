//
// Created by rgerst on 27.07.18.
//


#pragma once

#include <string>
#include <memory>
#include <vector>
#include <misaxx/workers/task_tree/misa_worker_status.h>
#include <unordered_set>
#include <any>
#include <cxxh/containers/singleton_map.h>
#include <misaxx/workers/task_tree/misa_work_subtree_status.h>
#include <misaxx/runtime/misa_runtime_base.h>
#include <misaxx/workers/paths/full_node_path.h>
#include <misaxx/parameters/parameter_base.h>
#include <misaxx/workers/misa_worker_base.h>
#include <memory>
#include <mutex>

namespace misaxx::nodes {

    /**
     * Internally used node in the task tree. A node is used to instantiate the actual worker.
     */
    class misa_work_node : public std::enable_shared_from_this<misa_work_node> {

    public:

        using instance_ptr_type = std::shared_ptr<misa_worker_base>;
        using instantiator_type = std::function<instance_ptr_type(const std::shared_ptr<misa_work_node>&)>;

        explicit misa_work_node(const std::string &t_name, const std::shared_ptr<misa_work_node> &t_parent, instantiator_type t_instantiator) : m_name(t_name),
                                                                         m_parent(t_parent),
                                                                         m_path(t_parent ? full_node_path(t_parent->m_path, t_name) : full_node_path(t_name)),
                                                                         m_json_path(m_path.to_string()),
                                                                         m_instantiator(std::move(t_instantiator)) {
        }

        const std::string &get_name() const {
            return m_name;
        }

        const std::weak_ptr<misa_work_node> get_parent() const {
            return m_parent;
        }

        /**
         * Gets a path from a custom path provider
         * @tparam Path
         * @return
         */
        template<class Path> const std::vector<std::string> &get_custom_path() const {
            if constexpr (std::is_same<Path, full_node_path>::value) {
                return get_path();
            }
            else {
                if(m_custom_paths.find<Path>() == m_custom_paths.end()) {
                    m_custom_paths.insert(Path(self()));
                }
                return m_custom_paths.at<Path>().get_path();
            }
        }

        /**
         * Gets the full path of this node
         * @return
         */
        const std::vector<std::string> &get_path() const {
            return m_path.get_path();
        }

        const std::string &get_json_path() const {
            return m_json_path;
        }

        /**
         * Returns true if this node's work can be parallelized
         * @return
         */
        bool is_parallelizeable() {
            return get_or_create_instance()->is_parallelizeable();
        }

        /**
         * Returns true if this node has all children. This means that the node iterator can stop watching this node for changes.
         * @return
         */
        misa_work_subtree_status get_subtree_status() const {
            return m_status == misa_worker_status::done ? misa_work_subtree_status ::complete : misa_work_subtree_status ::incomplete;
        }

        /**
         * Returns true if the node has no work to do
         * @return
         */
        misa_worker_status get_worker_status() const {
            return m_status;
        }

        /**
         * Allows the worker instance to reject work.
         */
        void reject_work() {
            m_status = misa_worker_status ::rejected;
        }

        /**
         * Allows the runtime to skip the work (will be set to done)
         */
        void skip_work() {
            m_status = misa_worker_status ::done;
        }

        /**
         * Starts the actual work of this node
         */
        void work() {
            if(m_status == misa_worker_status::undone || m_status == misa_worker_status::rejected) {
                m_status = misa_worker_status ::working;
                get_or_create_instance()->work();
                if(m_status != misa_worker_status::rejected) {
                    m_status = misa_worker_status ::done;
                }
            }
        }

        /**
         * Returns a pointer to the instance.
         * @return
         */
        std::shared_ptr<misa_worker_base> get_or_create_instance() {
            std::lock_guard<std::mutex> lock(m_instantiator_mutex);
            if(!m_instance) {
                m_instance = m_instantiator(self());
            }
            return m_instance;
        }

        std::shared_ptr<misa_worker_base> get_instance() const {
            return m_instance;
        }

        /**
         * Creates a parameter with given value and if needed, registers it in internal structures.
         * @tparam Parameter
         * @tparam T
         * @param t_value
         * @return
         */
        template<class Parameter, typename T = typename Parameter::value_type>
        Parameter make_parameter(T t_value) {
            return Parameter(std::move(t_value), self());
        }


        std::shared_ptr<misa_work_node> make_child(const std::string &t_name, instantiator_type t_instantiator) {
            auto ptr = std::make_shared<misa_work_node>(t_name, self(), std::move(t_instantiator));
            m_children.push_back(ptr);
            return ptr;
        }

        /**
         * Returns the list of children
         * @return
         */
        std::vector<std::shared_ptr<misa_work_node>> &get_children() {
            return m_children;
        }

        /**
         * Returns the list of children
         * @return
         */
        const std::vector<std::shared_ptr<misa_work_node>> &get_children() const {
            return m_children;
        }

        /**
         * Returns the list of dependencies this node has
         * @return
         */
        std::unordered_set<std::shared_ptr<misa_work_node>> &get_dependencies() {
            return m_dependencies;
        }

        /**
         * Returns true if all dependencies are satisfied
         * @return
         */
        bool dependencies_satisfied() {
            for(auto &dep : m_dependencies) {
                if(dep->get_worker_status() != misa_worker_status::done)
                    return false;
            }
            return true;
        }

        /**
        * Returns a managed pointer to this node
        * @return
        */
        std::shared_ptr<misa_work_node> self() {
            return shared_from_this();
        }

        /**
        * Returns a managed pointer to this node
        * @return
        */
        std::shared_ptr<const misa_work_node> self() const {
            return shared_from_this();
        }

    private:

        std::string m_name;
        std::weak_ptr<misa_work_node> m_parent;
        full_node_path m_path;
        std::string m_json_path;
        mutable cxxh::containers::singleton_map m_custom_paths;
        std::vector<std::shared_ptr<misa_work_node>> m_children;
        std::unordered_set<std::shared_ptr<misa_work_node>> m_dependencies;

        /**
         * Status of the worker
         */
        std::atomic<misa_worker_status> m_status = misa_worker_status::undone;

        /**
         * The actual worker instance
         */
        instance_ptr_type m_instance;

        /**
         * The instantiator responsible for creating the worker instance when requested
         */
        instantiator_type m_instantiator;

        /**
         * Required for locking the instance pointer when the instance is created
         */
        std::mutex m_instantiator_mutex;
    };
}
