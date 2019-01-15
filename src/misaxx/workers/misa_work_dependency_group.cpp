#include <misaxx/workers/misa_work_dependency_group.h>
#include <misaxx/misa_worker.h>

using namespace misaxx;

misa_work_dependency_group::misa_work_dependency_group(
        const std::initializer_list<std::reference_wrapper<misa_work_dependency_segment>> &t_segments) {
    for(const auto &seg : t_segments) {
        for(auto &nd : seg.get().to_dependencies()) {
            m_dependencies.insert(nd);
        }
    }
    m_as_dependencies = m_dependencies;
}

misa_work_dependency_group::misa_work_dependency_group(misaxx::misa_worker &t_worker) : m_as_dependencies( { t_worker.get_node() } ) {

}

depencencies_t misa_work_dependency_group::dependencies() const {
    return m_dependencies;
}

depencencies_t misa_work_dependency_group::to_dependencies() {
    m_locked = true;
    return m_as_dependencies;
}

void misa_work_dependency_group::assign(std::shared_ptr<misaxx::misa_work_node> t_node) {
    if(m_locked) {
        throw std::runtime_error("Cannot assign nodes to this group after it has been used as dependency!");
    }
    auto &nd = *t_node;
    m_as_dependencies.insert(std::move(t_node));
    nd.get_dependencies() = m_dependencies;
    m_dependency_locked = true;
}

void misa_work_dependency_group::add_dependency(misa_work_dependency_segment &t_segment) {
    if(m_locked) {
        throw std::runtime_error("Cannot assign dependencies to this group after it has been used as dependency!");
    }
    if(m_dependency_locked) {
        throw std::runtime_error("Cannot assign dependencies to this group after it was assigned worker nodes!");
    }

    for(auto &nd : t_segment.to_dependencies()) {
        m_dependencies.insert(nd);
        m_as_dependencies.insert(nd);
    }
}
