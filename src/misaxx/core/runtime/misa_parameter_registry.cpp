//
// Created by rgerst on 16.01.19.
//

#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <src/misaxx/core/runtime/misa_runtime.h>

using namespace misaxx;

nlohmann::json &misaxx::parameter_registry::get_parameter_json() {
    return misa_runtime::instance().get_parameter_json();
}

nlohmann::json misaxx::parameter_registry::get_json_raw(const std::vector<std::string> &t_path) {
    return misa_runtime::instance().get_json_raw(t_path);
}

misa_json_schema_builder &misaxx::parameter_registry::get_schema_builder() {
    return misa_runtime::instance().get_schema_builder();
}
