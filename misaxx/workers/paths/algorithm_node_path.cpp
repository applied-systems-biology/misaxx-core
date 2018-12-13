#include <misaxx/workers/paths/algorithm_node_path.h>

using namespace misaxx;

algorithm_node_path::algorithm_node_path(const std::shared_ptr<const misaxx::nodes::misa_work_node> &t_node) : misaxx::guarded_node_path<misa_root_module_base>::guarded_node_path("algorithm", t_node) {
    // Erase object name from path
    m_path.erase(m_path.begin() + 1);
}
