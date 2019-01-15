#include <misaxx/workers/misa_dispatcher.h>
#include <misaxx/misa_dispatcher_builder.h>

using namespace misaxx;

std::vector<misa_dispatcher::blueprint> misa_dispatcher::export_to_algorithm_json(const std::string &t_param_name,
                                                                                  std::vector<misa_dispatcher::blueprint> t_blueprints,
                                                                                  const std::optional<std::string> &t_default) {
    misa_json_property <std::string> m;
    for(const auto &bp : t_blueprints) {
        m.allowed_values.push_back(bp->get_name());
    }
    m.default_value = t_default;
    this->template from_algorithm_json<std::string>(t_param_name, std::move(m));
    return t_blueprints;
}

void misa_dispatcher::build_simulation(const misa_dispatcher::builder &t_builder) {
    for(const auto &kv : t_builder.get_entries()) {
        kv.second->dispatch(*this);
    }
}

void misa_dispatcher::execute_work() {
    // We need to create the builder here when we have an already determined adress
    this->m_builder = std::make_unique<misa_dispatcher_builder>(*this);
    this->create_blueprints(*m_builder);
    if (misa_runtime_base::instance().is_simulating())
        this->build_simulation(*m_builder);
    else
        this->build(*m_builder);
}

bool misa_dispatcher::is_parallelizeable() const {
    return false;
}
