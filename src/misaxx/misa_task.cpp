#include <misaxx/misa_task.h>

using namespace misaxx;

misa_task::misa_task(const misa_worker::node &t_node, const misa_worker::module &t_module) : misa_worker(t_node, t_module) {
    auto is_parallelizeable_path = t_node->get_algorithm_path()->get_path();
    is_parallelizeable_path.emplace_back("task::is_parallelizeable");
    misa_json_property<bool> prop;
    prop.default_value = true;
    prop.title = "Is Parallelizable";
    prop.description = "If enabled, this task can be run in parallel";
    misaxx::parameter_registry::register_parameter(is_parallelizeable_path, prop);
    is_parallelizeable_parameter = misa_parameter<bool>(std::move(is_parallelizeable_path), std::move(prop));
}

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

void misa_task::create_parameters(parameter_list &) {
}

const misa_parameter_builder &misa_task::get_parameters() const {
    return *m_parameter_builder;
}


