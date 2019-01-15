//
// Created by rgerst on 27.07.18.
//


#pragma once

#include <string>
#include <memory>
#include <vector>
#include <misaxx/workers/misa_worker_status.h>
#include <unordered_set>
#include <any>
#include <misaxx-helpers/singleton_map.h>
#include <misaxx/workers/misa_work_subtree_status.h>
#include <misaxx/runtime/misa_runtime_base.h>
#include <misaxx/misa_parameter_base.h>
#include <memory>
#include <mutex>
#include <misaxx/workers/misa_work_tree_node_path.h>
#include <misaxx/workers/misa_work_tree_algorithm_path.h>
#include <misaxx/workers/misa_work_tree_sample_path.h>

namespace misaxx {

    struct misa_worker;

    /**
     * Internally used node in the task tree. A node is used to instantiate the actual worker.
     */
    class misa_work_node : public std::enable_shared_from_this<misa_work_node> {

    public:

        using instance_ptr_type = std::shared_ptr<misa_worker>;
        using instantiator_type = std::function<instance_ptr_type(const std::shared_ptr<misa_work_node>&)>;

        explicit misa_work_node(const std::string &t_name, const std::shared_ptr<misa_work_node> &t_parent, instantiator_type t_instantiator);

        /**
         * Gets the name of this node
         * @return
         */
        const std::string &get_name() const;

        /**
         * Gets the parent of this node
         * @return
         */
        const std::weak_ptr<misa_work_node> get_parent() const;

        /**
         * Gets the full path of this node
         * @return
         */
        std::shared_ptr<const misa_work_tree_node_path> get_global_path() const;

        /**
         * Gets the algorithm path of this node
         * @return
         */
        std::shared_ptr<const misa_work_tree_algorithm_path> get_algorithm_path() const;

        /**
         * Gets the object path of this node
         * @return
         */
        std::shared_ptr<const misa_work_tree_sample_path> get_sample_path() const;

        /**
         * Returns true if this node's work can be parallelized
         * @return
         */
        bool is_parallelizeable();

        /**
         * Returns true if this node has all children. This means that the node iterator can stop watching this node for changes.
         * @return
         */
        misa_work_subtree_status get_subtree_status() const;

        /**
         * Returns true if the node has no work to do
         * @return
         */
        misa_worker_status get_worker_status() const;

        /**
         * Allows the worker instance to reject work.
         */
        void reject_work();

        /**
         * Allows the runtime to skip the work (will be set to done)
         */
        void skip_work();

        /**
         * Starts the actual work of this node
         */
        void work();

        /**
         * Returns a pointer to the instance. If necessary, create the instance
         * @return
         */
        std::shared_ptr<misa_worker> get_or_create_instance();

        /**
         * Returns a pointer to the instance.
         * @return
         */
        std::shared_ptr<misa_worker> get_instance() const;

        /**
         * Create a child node
         * @param t_name
         * @param t_instantiator
         * @return
         */
        std::shared_ptr<misa_work_node> make_child(const std::string &t_name, instantiator_type t_instantiator);

        /**
         * Returns the list of children
         * @return
         */
        std::vector<std::shared_ptr<misa_work_node>> &get_children();

        /**
         * Returns the list of children
         * @return
         */
        const std::vector<std::shared_ptr<misa_work_node>> &get_children() const;

        /**
         * Returns the list of dependencies this node has
         * @return
         */
        std::unordered_set<std::shared_ptr<misa_work_node>> &get_dependencies();

        /**
         * Returns true if all dependencies are satisfied
         * @return
         */
        bool dependencies_satisfied();

        /**
        * Returns a managed pointer to this node
        * @return
        */
        std::shared_ptr<misa_work_node> self();

        /**
        * Returns a managed pointer to this node
        * @return
        */
        std::shared_ptr<const misa_work_node> self() const;

    private:

        /**
         * Name of this node
         */
        std::string m_name;

        /**
         * Pointer to the parent
         */
        std::weak_ptr<misa_work_node> m_parent;

        /**
         * Child nodes
         */
        std::vector<std::shared_ptr<misa_work_node>> m_children;

        /**
         * Nodes that are dependencies of this node
         */
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

        mutable std::shared_ptr<misa_work_tree_node_path> m_global_path;

        mutable std::shared_ptr<misa_work_tree_algorithm_path> m_algorithm_path;

        mutable std::shared_ptr<misa_work_tree_sample_path> m_object_path;
    };
}
