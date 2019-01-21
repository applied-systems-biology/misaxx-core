//
// Created by rgerst on 16.01.19.
//

#include <misaxx/core/runtime/misa_runtime_properties.h>
#include <misaxx/core/runtime/misa_runtime_base.h>

using namespace misaxx;

int misaxx::runtime_properties::get_num_threads() {
    return misa_runtime_base::instance().get_num_threads();
}

bool misaxx::runtime_properties::is_simulating() {
    return misa_runtime_base::instance().is_simulating();
}

bool misaxx::runtime_properties::is_running() {
    return misa_runtime_base::instance().is_running();
}

std::shared_ptr<misa_work_node> misaxx::runtime_properties::get_root_node() {
    return misa_runtime_base::instance().get_root_node();
}

misa_filesystem misaxx::runtime_properties::get_root_filesystem() {
    return misa_runtime_base::instance().get_filesystem();
}

misa_module_info runtime_properties::get_module_info() {
    return misa_runtime_base::instance().get_module_info();
}
