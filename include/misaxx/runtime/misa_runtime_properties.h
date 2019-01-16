//
// Created by rgerst on 16.01.19.
//


#pragma once

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include <misaxx/misa_module_info.h>

namespace misaxx {
    struct misa_cache;
    struct misa_json_schema_builder;
    struct misa_work_node;
    struct misa_filesystem;
}

/**
 * Helper namespace that allows access to some properties of the current runtime
 * This allows querying the runtime without depending on its header
 */
namespace misaxx::runtime_properties {

    /**
     * Returns the number of threads
     * @return
     */
    extern int get_num_threads();

    /**
     * If true, the runtime is in simulation mode and no actual work should be done
     * @return
     */
    extern bool is_simulating();

    /**
     * Returns true if the runtime is currently working
     * @return
     */
    extern bool is_running();

    /**
     * Returns the root node of the worker tree
     * @return
     */
    extern std::shared_ptr<misa_work_node> get_root_node();

    /**
     * Returns the filesystem of the root module
     * Please note that the root module is usually misa_multiobject_root.
     * @return
     */
    extern misa_filesystem get_root_filesystem();

    /**
     * Returns the module information that was attached to the runtime
     * @return
     */
    extern misa_module_info get_module_info();
}