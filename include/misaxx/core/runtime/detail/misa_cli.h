//
// Created by rgerst on 29.03.19.
//

#pragma once

#include <misaxx/core/workers/misa_work_node.h>

namespace misaxx {
    template<class Module>
    void misa_cli::set_root_module(const std::string &t_name) {
        auto instantiator = [](const std::shared_ptr<misaxx::misa_work_node> &node) {
            return misaxx::misa_work_node::instance_ptr_type(
                    new Module(node));
        };
        std::shared_ptr<misa_work_node> root_node = misa_work_node::create_instance(t_name,
                                                                                    std::shared_ptr<misa_work_node>(),
                                                                                    instantiator);
        this->set_root_node(std::move(root_node));
    }
}