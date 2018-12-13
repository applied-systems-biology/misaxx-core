#include <misaxx/workers/paths/full_node_path.h>

using namespace misaxx;

full_node_path::full_node_path(std::string t_segment) : m_path({ std::move(t_segment) }) {
}

full_node_path::full_node_path(const full_node_path &t_parent, std::string t_segment) : m_path(sub_path(t_parent, std::move(t_segment))) {
}

const std::vector<std::string> &full_node_path::get_path() const {
    return m_path;
}

std::string full_node_path::to_string(const std::string &t_separator) const {
    return boost::algorithm::join(m_path, t_separator);
}

std::vector<std::string> full_node_path::sub_path(const full_node_path &t_parent, std::string t_segment) {
    std::vector<std::string> result = t_parent.m_path;
    result.emplace_back(std::move(t_segment));
    return result;
}
