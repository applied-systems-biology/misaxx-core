//
// Created by rgerst on 04.09.18.
//


#pragma once
#include <misaxx/workers/task_tree/misa_work_node.h>
#include <functional>
#include <boost/algorithm/string/join.hpp>

namespace misaxx {
    /**
     * Node path that has the nearest parent of given guard type has root.
     * @tparam Guard
     */
    template<class Guard> struct guarded_node_path {
        guarded_node_path() = default;

        explicit guarded_node_path(const std::string &t_name, const std::shared_ptr<const nodes::misa_work_node> &t_node) {
            std::shared_ptr<const nodes::misa_work_node> ptr = t_node;
            while(ptr != nullptr) {
                if(dynamic_cast<const Guard*>(ptr->get_instance().get())) {
                    break;
                }
                m_path.push_back(ptr->get_name());
                if(ptr->get_parent().expired())
                    break;
                ptr = ptr->get_parent().lock();
            }
            m_path.push_back(t_name);
            std::reverse(m_path.begin(), m_path.end());
        }

        const std::vector<std::string> &get_path() const {
            return m_path;
        }

        std::string to_string(const std::string &t_separator = "/") const {
            return boost::algorithm::join(m_path, t_separator);
        }

    protected:
        std::vector<std::string> m_path;
    };
}
