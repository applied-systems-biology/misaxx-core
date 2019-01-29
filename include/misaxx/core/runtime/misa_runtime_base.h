//
// Created by rgerst on 30.07.18.
//


#pragma once

#include <vector>
#include <string>
#include <boost/algorithm/string/join.hpp>
#include <misaxx/core/json/misa_json_property.h>
#include <nlohmann/json.hpp>
#include <misaxx/core/json/misa_json_schema_builder.h>
#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/misa_primitive.h>
#include <misaxx/core/misa_cache.h>
#include <unordered_set>
#include <misaxx/core/misa_module_info.h>

namespace misaxx {

    struct misa_work_node;

    /**
     * The runtime is responsible for executing the work represented by the worker tree and providing external parameters such as
     * JSON parameters.
     * This is the interface containing common functions for different runtime implementations.
     */
    struct misa_runtime_base {

    private:

        std::shared_ptr<misa_work_node> m_root;

        std::vector<std::shared_ptr<misa_cache>> m_registered_caches;

        size_t m_known_nodes_count = 0;

        size_t m_finished_nodes_count = 0;

        bool m_tree_complete = false;

        /**
         * List of nodes that are not completed and need to be watched
         */
        std::vector<misa_work_node *> m_nodes_todo;
        /**
         * List of nodes for fast lookup of unknown nodes
         */
        std::unordered_set<misa_work_node *> m_nodes_todo_lookup;

        // Variables are protected and will be set by the inheriting runtime

        bool m_is_simulating = false;

        /**
       * Number of threads used by the runtime.
       * If the number is 1, the application will run in the current thread.
       */
        int m_num_threads = 1;

        /**
         * Parameters for the workers
         */
        nlohmann::json m_parameters;

        /**
         * Used to create a configuration schema
         */
        misa_json_schema_builder m_parameter_schema_builder;

    protected:

        /**
         * Protected constructor for singleton
         */
        misa_runtime_base() = default;

        virtual ~misa_runtime_base() = default;

        virtual std::shared_ptr<misa_work_node> create_root_node() = 0;

    public:

        /**
         * Enables or disables simulation mode
         * @param value
         */
        void set_is_simulating(bool value);

        /**
         * Sets the number of threads
         * @param threads
         */
        void set_num_threads(int threads);

        /**
         * Returns the number of threads
         * @return
         */
        int get_num_threads() const;

        /**
         * Returns true if the runtime is in simulation mode
         * @return
         */
        bool is_simulating() const;

        /**
         * Returns true if this runtime is currently working
         * @return
         */
        bool is_running() const;

        /**
         * Registers a cache into this runtime (e.g. used for attachment export)
         * @param t_cache
         */
        void register_cache(std::shared_ptr<misa_cache> t_cache);

        /**
       * Returns the list of registered caches
       * @return
       */
        const std::vector<std::shared_ptr<misa_cache>> &get_registered_caches() const;

        /**
         * Returns the JSON that contains the parameters
         * @return
         */
        nlohmann::json &get_parameter_json();

        /**
         * Sets the parameter JSON
         * @param t_json
         */
        void set_parameter_json(nlohmann::json t_json);

        /**
         * Gets the raw JSON value of a path
         * @param t_path
         * @return
         */
        nlohmann::json get_json_raw(const std::vector<std::string> &t_path);

        /**
         * Returns an optional instance to a schema builder.
         * If it returns nullptr, the schema builder will be ignored.
         * @return
         */
        misa_json_schema_builder &get_schema_builder();

        /**
         * Returns the root node
         * @return
         */
        std::shared_ptr<misa_work_node> get_root_node() const;

        /**
         * Returns the current filesystem
         * @return
         */
        virtual misa_filesystem get_filesystem() = 0;

        /**
         * Sets the filesystem
         * @param t_filesystem
         */
        virtual void set_filesystem(misa_filesystem t_filesystem) = 0;

        /**
         * Returns the module info
         * @return
         */
        virtual misa_module_info get_module_info() = 0;

        /**
         * Starts the actual work of the runtime.
         */
        void run();

    private:

        void run_single_threaded();

        void run_parallel();

        void progress(const std::string &t_text);

        void progress(const misa_work_node &t_node, const std::string &t_text);

        void postprocess_caches();

        void postprocess_cache_attachments();

        void postprocess_parameter_schema();

    protected:

        /**
         * Static singleton instance of the runtime
         */
        static std::weak_ptr<misa_runtime_base> singleton_instance;

    public:

        /**
         * Returns the current runtime
         * @return
         */
        static misa_runtime_base &instance();
    };

}