//
// Created by rgerst on 15.01.19.
//

#include <misaxx/core/workers/misa_work_tree_node_path.h>
#include <misaxx/core/workers/misa_work_node.h>

using namespace misaxx;

misa_work_tree_node_path::misa_work_tree_node_path(const std::shared_ptr<const misa_work_node> &t_node) {
    auto parent = t_node->get_parent().lock();
    if(static_cast<bool>(parent)) {
        m_node_path = parent->get_global_path()->m_node_path;
        m_path = parent->get_global_path()->m_path;
    }

    m_node_path.push_back(t_node);
    m_path.push_back(t_node->get_name());
}

const std::vector<std::weak_ptr<const misa_work_node>> &misa_work_tree_node_path::get_node_path() const {
    return m_node_path;
}

const std::vector<std::string> &misa_work_tree_node_path::get_path() const {
    return m_path;
}
