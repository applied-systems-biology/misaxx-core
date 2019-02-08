//
// Created by rgerst on 15.01.19.
//


#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <ostream>
#include <boost/algorithm/string/join.hpp>

namespace misaxx {

    class misa_work_node;

    /**
     * Base class for worker tree node paths
     */
    struct misa_work_tree_path {

        /**
         * Returns the hierarchy of nodes
         * @return
         */
        virtual const std::vector<std::weak_ptr<const misa_work_node>> &get_node_path() const = 0;

        /**
         * Returns the node names
         * @return
         */
        virtual const std::vector<std::string> &get_path() const = 0;

        friend std::ostream &operator<<(std::ostream &os, const misa_work_tree_path &path) {
            os << boost::algorithm::join(path.get_path(), "/");
            return os;
        }
    };

}
