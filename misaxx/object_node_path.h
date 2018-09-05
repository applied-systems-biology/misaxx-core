//
// Created by rgerst on 05.09.18.
//


#pragma once
#include <pattxx/guarded_node_path.h>
#include "root_modules/misa_root_module_base.h"

namespace misaxx {

    /**
     * Path for object parameters. Global in all sub-nodes
     */
    struct object_node_path : public pattxx::guarded_node_path<misa_root_module_base> {
        object_node_path() = default;
        explicit object_node_path(const pattxx::nodes::node &t_node) : pattxx::guarded_node_path<misa_root_module_base>::guarded_node_path("objects", t_node) {
            // Global within object
            m_path.resize(2);
        }
    };
}
