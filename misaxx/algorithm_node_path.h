//
// Created by rgerst on 04.09.18.
//


#pragma once
#include <pattxx/guarded_node_path.h>
#include "root_modules/misa_root_module_base.h"

namespace misaxx {

    /**
     * Path for algorithm parameters. Will ignore the object name
     */
    struct algorithm_node_path : public pattxx::guarded_node_path<misa_root_module_base> {
        algorithm_node_path() = default;
        explicit algorithm_node_path(const pattxx::nodes::node &t_node) : pattxx::guarded_node_path<misa_root_module_base>::guarded_node_path("algorithm", t_node) {
            // Erase object name from path
            m_path.erase(m_path.begin() + 1);
        }
    };
}
