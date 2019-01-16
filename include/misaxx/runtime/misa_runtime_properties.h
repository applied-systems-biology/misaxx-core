//
// Created by rgerst on 16.01.19.
//


#pragma once

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

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

    extern int get_num_threads();

    extern bool is_simulating();

    extern bool is_running();

    extern std::shared_ptr<misa_work_node> get_root_node();

    extern misa_filesystem get_root_filesystem();
}