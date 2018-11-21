//
// Created by rgerst on 05.09.18.
//


#pragma once
#include <misaxx/misa_root_module_base.h>
#include <misaxx/workers/paths/guarded_node_path.h>
#include <misaxx/workers/task_tree/misa_work_node.h>

namespace misaxx {

    /**
     * Path for object parameters. Global in all sub-nodes
     */
    struct object_node_path : public guarded_node_path<misa_root_module_base> {
        object_node_path() = default;
        explicit object_node_path(const std::shared_ptr<const nodes::misa_work_node> &t_node) : misaxx::guarded_node_path<misa_root_module_base>::guarded_node_path("objects", t_node) {
            // Global within object
            m_path.resize(2);
        }
    };
}
