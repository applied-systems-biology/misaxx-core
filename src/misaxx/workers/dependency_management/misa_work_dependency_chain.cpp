#include <misaxx/workers/dependency_management/misa_work_dependency_chain.h>

using namespace misaxx::dependencies;

misa_work_dependency_chain::misa_work_dependency_chain(
        const std::initializer_list<std::reference_wrapper<misa_work_dependency_segment>> &t_segments) {
    for (const auto &seg : t_segments) {
        for (auto &nd : seg.get().to_dependencies()) {
            m_dependencies.insert(nd);
        }
    }
    m_as_dependencies = m_dependencies;
    m_consecutive_dependencies = m_dependencies;
}

depencencies_t misa_work_dependency_chain::dependencies() const {
    return m_dependencies;
}

depencencies_t misa_work_dependency_chain::to_dependencies() {
    m_locked = true;
    return m_as_dependencies;
}

void misa_work_dependency_chain::assign(std::shared_ptr<misaxx::nodes::misa_work_node> t_node) {
    if (m_locked) {
        throw std::runtime_error("Cannot assign nodes to this chain after it has been used as dependency!");
    }
    m_as_dependencies.insert(t_node);
    t_node->get_dependencies() = m_consecutive_dependencies;
    m_consecutive_dependencies.insert(std::move(t_node));
}

void misa_work_dependency_chain::add_dependency(misa_work_dependency_segment &t_segment) {
    for (auto &nd : t_segment.to_dependencies()) {
        m_consecutive_dependencies.insert(nd);
    }
}
