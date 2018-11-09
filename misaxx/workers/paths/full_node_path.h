//
// Created by rgerst on 04.09.18.
//


#pragma once
#include <vector>
#include <string>
#include <boost/algorithm/string/join.hpp>

namespace misaxx {
    /**
     * Stores the full path of a node.
     * To create custom paths, use the guarded_node_path class or create a custom path class
     * that has a method get_path() and to_string().
     * Additionally, the custom path class should have a constructor taking a const pattxx::nodes::misa_work_node&
     */
    class full_node_path {
    public:
        full_node_path() = default;

        explicit full_node_path(std::string t_segment) : m_path({ std::move(t_segment) }) {
        }

        explicit full_node_path(const full_node_path &t_parent, std::string t_segment) : m_path(sub_path(t_parent, std::move(t_segment))) {
        }

        const std::vector<std::string> &get_path() const {
            return m_path;
        }

        std::string to_string(const std::string &t_separator = "/") const {
            return boost::algorithm::join(m_path, t_separator);
        }

    private:
        std::vector<std::string> m_path;

        static std::vector<std::string> sub_path(const full_node_path &t_parent, std::string t_segment) {
            std::vector<std::string> result = t_parent.m_path;
            result.emplace_back(std::move(t_segment));
            return result;
        }
    };
}
