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

        explicit full_node_path(std::string t_segment);

        explicit full_node_path(const full_node_path &t_parent, std::string t_segment);

        const std::vector<std::string> &get_path() const;

        std::string to_string(const std::string &t_separator = "/") const;

    private:
        std::vector<std::string> m_path;

        static std::vector<std::string> sub_path(const full_node_path &t_parent, std::string t_segment);
    };
}
