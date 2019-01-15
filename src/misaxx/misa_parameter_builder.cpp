//
// Created by rgerst on 15.01.19.
//

#include <misaxx/misa_parameter_builder.h>
#include <misaxx/misa_worker.h>

using namespace misaxx;

misa_parameter_builder::misa_parameter_builder(misa_worker &t_worker) : m_worker(t_worker.self()){

}

std::vector<std::string> misa_parameter_builder::get_algorithm_path() {
    return m_worker.lock()->get_node()->get_algorithm_path()->get_path();
}

std::vector<std::string> misa_parameter_builder::get_sample_path() {
    return m_worker.lock()->get_node()->get_sample_path()->get_path();
}

std::vector<std::string> misa_parameter_builder::get_node_path() {
    return m_worker.lock()->get_node()->get_global_path()->get_path();
}

std::vector<std::string> misa_parameter_builder::get_runtime_path() {
    return {{ "runtime" }};
}
