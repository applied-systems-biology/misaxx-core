#include <misaxx/misa_dispatcher.h>
#include <misaxx/misa_dispatcher_builder.h>
#include <misaxx-helpers/ref.h>

using namespace misaxx;

void misa_dispatcher::build_simulation(const misa_dispatcher::blueprint_builder &t_builder) {
    for(const auto &kv : t_builder.get_entries()) {
        kv.second->dispatch(*this);
    }
}

void misa_dispatcher::execute_work() {
    // We need to create the builder here when we have an already determined adress
    this->m_builder = std::make_unique<misa_dispatcher_builder>(*this);
    this->m_parameter_builder = std::make_unique<misa_parameter_builder>(*this);
    this->create_parameters(*m_parameter_builder);
    this->create_blueprints(*m_builder, *m_parameter_builder);
    if (misa_runtime_base::instance().is_simulating())
        this->build_simulation(*m_builder);
    else
        this->build(*m_builder);
}

bool misa_dispatcher::is_parallelizeable() const {
    return false;
}

std::vector<misa_dispatcher::blueprint>
misa_dispatcher::create_blueprint_enum_parameter(misa_parameter<std::string> &t_parameter,
                                                 std::vector<misa_dispatcher::blueprint> t_blueprints,
                                                 const std::optional<std::string> &t_default) {
    for(const auto &bp : t_blueprints) {
        t_parameter.allowed_values.push_back(bp->get_name());
    }
    if(t_default.has_value()) {
        t_parameter.with_default_value(t_default.value());
    }

    // Re-register into the runtime
    misa_runtime_base::instance().register_parameter(t_parameter.get_location(), t_parameter);

    return std::move(t_blueprints);
}

const misa_parameter_builder &misa_dispatcher::get_parameters() const {
    return *m_parameter_builder;
}

void misa_dispatcher::create_parameters(misa_parameter_builder &) {

}