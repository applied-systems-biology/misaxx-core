#include <misaxx/workers/paths/object_node_path.h>

using namespace misaxx;

object_node_path::object_node_path(const std::shared_ptr<const nodes::misa_work_node> &t_node) : guarded_node_path<misa_root_module_base>::guarded_node_path("objects", t_node) {
    // Global within object
    m_path.resize(2);
}
