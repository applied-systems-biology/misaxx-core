#include <misaxx/workers/task_tree/misa_work_node.h>

using namespace misaxx::nodes;

misa_work_node::misa_work_node(const std::string &t_name,
                                              const std::shared_ptr<misa_work_node> &t_parent,
                                              misa_work_node::instantiator_type t_instantiator) : m_name(t_name),
                                                                                                                 m_parent(t_parent),
                                                                                                                 m_path(t_parent ? full_node_path(t_parent->m_path, t_name) : full_node_path(t_name)),
                                                                                                                 m_json_path(m_path.to_string()),
                                                                                                                 m_instantiator(std::move(t_instantiator)) {
}

const std::string &misa_work_node::get_name() const {
    return m_name;
}

const std::weak_ptr<misa_work_node> misa_work_node::get_parent() const {
    return m_parent;
}

const std::vector<std::string> &misa_work_node::get_path() const {
    return m_path.get_path();
}

const std::string &misa_work_node::get_json_path() const {
    return m_json_path;
}

bool misa_work_node::is_parallelizeable() {
    return get_or_create_instance()->is_parallelizeable();
}

misa_work_subtree_status misa_work_node::get_subtree_status() const {
    if(m_status == misa_worker_status::done) {
        return misa_work_subtree_status ::complete;
    }
    else if(m_status == misa_worker_status::waiting) {
        for(const auto &child : m_instance->get_node()->get_children()) {
            if(child->get_subtree_status() != misa_work_subtree_status ::complete) {
                return misa_work_subtree_status::incomplete;
            }
        }
        return misa_work_subtree_status ::complete;
    }
    else {
        return misa_work_subtree_status::incomplete;
    }
}

misa_worker_status misa_work_node::get_worker_status() const {
    // Check the status of the worker if it is waiting for subworkers
    if(m_status == misa_worker_status::waiting) {
        if(!static_cast<bool>(m_instance)) {
            throw std::logic_error("The worker is working without an instance!");
        }

        for(const auto &child : m_instance->get_node()->get_children()) {
            if(child->get_worker_status() != misa_worker_status::done) {
                return misa_worker_status::working;
            }
        }

        return misa_worker_status::done;
    }
    return m_status;
}

void misa_work_node::reject_work() {
    m_status = misa_worker_status ::rejected;
}

void misa_work_node::skip_work() {
    m_status = misa_worker_status ::done;
}

void misa_work_node::work() {
    if(m_status == misa_worker_status::undone || m_status == misa_worker_status::rejected) {
        auto instance = get_or_create_instance();
        m_status = misa_worker_status ::working;
        instance->work();
        if(m_status != misa_worker_status::rejected) {
            // If the worker has no children, we can already declare that it is finished
            if(instance->get_node()->get_children().empty())
                m_status = misa_worker_status ::done;
            else
                m_status = misa_worker_status ::waiting;
        }
    }
}

std::shared_ptr<misaxx::misa_worker_base> misa_work_node::get_or_create_instance() {
    std::lock_guard<std::mutex> lock(m_instantiator_mutex);
    if(!m_instance) {
        m_instance = m_instantiator(self());
    }
    return m_instance;
}

std::shared_ptr<misa_work_node>
misa_work_node::make_child(const std::string &t_name, misa_work_node::instantiator_type t_instantiator) {
    auto ptr = std::make_shared<misa_work_node>(t_name, self(), std::move(t_instantiator));
    m_children.push_back(ptr);
    return ptr;
}

std::vector<std::shared_ptr<misa_work_node>> &misa_work_node::get_children() {
    return m_children;
}

const std::vector<std::shared_ptr<misa_work_node>> &misa_work_node::get_children() const {
    return m_children;
}

std::unordered_set<std::shared_ptr<misa_work_node>> &misa_work_node::get_dependencies() {
    return m_dependencies;
}

bool misa_work_node::dependencies_satisfied() {
    for(auto &dep : m_dependencies) {
        if(dep->get_worker_status() != misa_worker_status::done)
            return false;
    }
    return true;
}

std::shared_ptr<misa_work_node> misa_work_node::self() {
    return shared_from_this();
}

std::shared_ptr<const misa_work_node> misa_work_node::self() const {
    return shared_from_this();
}
