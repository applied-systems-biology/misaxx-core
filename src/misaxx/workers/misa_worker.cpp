#include <misaxx/workers/misa_worker.h>

using namespace misaxx;

misaxx::misa_worker::misa_worker(const std::shared_ptr<misaxx::nodes::misa_work_node> &t_node,
                                 const misaxx::misa_worker::module &t_module_declaration) :
        m_node(t_node), m_module_declaration(t_module_declaration) {
}

misaxx::misa_worker::module misaxx::misa_worker::get_module() {
    return m_module_declaration.lock();
}
