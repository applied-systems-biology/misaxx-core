#include <misaxx/misa_task.h>

using namespace misaxx;

void misa_task::simulate_work() {
}

void misa_task::execute_work() {
    m_parameter_builder = std::make_unique<misa_parameter_builder>(*this);
    create_parameters(*m_parameter_builder);
    if(misaxx::runtime_properties::is_simulating())
        simulate_work();
    else
        work();
}

bool misa_task::is_parallelizeable() const {
    return is_parallelizeable_parameter.query();
}

void misa_task::create_parameters(parameter_list &t_parameters) {
    is_parallelizeable_parameter = t_parameters.create_algorithm_parameter<bool>("task::is_parallelizeable", true);
}

const misa_parameter_builder &misa_task::get_parameters() const {
    return *m_parameter_builder;
}
