//
// Created by rgerst on 15.01.19.
//

#include <misaxx/workers/misa_work_tree_algorithm_path.h>
#include <misaxx/workers/misa_work_node.h>
#include <misaxx/misa_worker.h>
#include <misaxx/misa_module_interface.h>

using namespace misaxx;

misa_work_tree_algorithm_path::misa_work_tree_algorithm_path(const std::shared_ptr<const misa_work_node> &t_node) {
    // If the node instance is a module interface, set the path to "algorithm"
    // Otherwise just add as with the global path
    if(static_cast<bool>(dynamic_cast<misa_module_interface*>(t_node->get_instance().get()))) {
        m_node_path.push_back(t_node);
        m_path.emplace_back("algorithm");
    }
    else {

        auto parent = t_node->get_parent().lock();
        if(static_cast<bool>(parent)) {
            // Append to path of parent
            m_node_path = parent->get_algorithm_path()->m_node_path;
            m_path = parent->get_algorithm_path()->m_path;
        }

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
