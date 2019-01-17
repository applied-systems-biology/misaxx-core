//
// Created by rgerst on 15.01.19.
//

#include <misaxx/workers/misa_work_tree_algorithm_path.h>
#include <misaxx/workers/misa_work_node.h>
#include <misaxx/misa_worker.h>
#include <misaxx/misa_module_interface.h>
#include <misaxx/misa_root_module_base.h>

using namespace misaxx;

misa_work_tree_algorithm_path::misa_work_tree_algorithm_path(const std::shared_ptr<const misa_work_node> &t_node) {
    auto parent = t_node->get_parent().lock();
    if(!static_cast<bool>(parent)) {
        // Do nothing here
    }
    else if(static_cast<bool>(dynamic_cast<misa_root_module_base*>(parent->get_instance().get()))) {
        // If the parent is a root module, set the root of the path to "algorithm"
        m_node_path.push_back(t_node);
        m_path.emplace_back("algorithm");
    }
    else {
        // Take the last algorithm path and continue from there
        m_node_path = parent->get_algorithm_path()->m_node_path;
        m_path = parent->get_algorithm_path()->m_path;
        m_node_path.push_back(t_node);
        m_path.push_back(t_node->get_name());
    }
}

const std::vector<std::weak_ptr<const misa_work_node>> &misa_work_tree_algorithm_path::get_node_path() const {
    return m_node_path;
}

const std::vector<std::string> &misa_work_tree_algorithm_path::get_path() const {
    return m_path;
}
