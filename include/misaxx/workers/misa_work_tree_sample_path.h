//
// Created by rgerst on 15.01.19.
//

#pragma once

#include <misaxx/workers/misa_work_tree_path.h>

namespace misaxx {
    class misa_work_tree_sample_path : public misa_work_tree_path {
    public:
        misa_work_tree_sample_path() = default;

        explicit misa_work_tree_sample_path(const std::shared_ptr<const misa_work_node> &t_node);

        const std::vector<std::weak_ptr<const misa_work_node>> &get_node_path() const override;

        const std::vector<std::string> &get_path() const override;

        friend std::ostream &operator<<(std::ostream &os, const misa_work_tree_sample_path &path) {
            os << boost::algorithm::join(path.get_path(), "/");
            return os;
        }

    private:

        std::vector<std::weak_ptr<const misa_work_node>> m_node_path;
        std::vector<std::string> m_path;
    };
}




