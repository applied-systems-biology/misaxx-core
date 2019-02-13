#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_parameter_builder.h>

using namespace misaxx;

misaxx::misa_worker::misa_worker(const std::shared_ptr<misaxx::misa_work_node> &t_node,
                                 const misaxx::misa_worker::module &t_module) :
        m_node(t_node), m_module(t_module) {
}

misaxx::misa_worker::module misaxx::misa_worker::get_module() {
    return m_module.lock();
}

std::shared_ptr<misa_work_node> misa_worker::get_node() const {
    return m_node.lock();
}

void misa_worker::repeat_work() {
    get_node()->repeat_work();
}

std::shared_ptr<misa_worker> misa_worker::self() const {
    return get_node()->get_instance();
}