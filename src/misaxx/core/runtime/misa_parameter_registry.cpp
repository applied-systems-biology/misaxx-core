//
// Created by rgerst on 16.01.19.
//

#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <misaxx/core/runtime/misa_runtime.h>

using namespace misaxx;

nlohmann::json &misaxx::parameter_registry::get_parameter_json() {
    return misa_runtime::instance().get_parameters();
}

nlohmann::json misaxx::parameter_registry::get_json_raw(const std::vector<std::string> &t_path) {
    return misa_runtime::instance().get_parameter_value(t_path);
}

std::shared_ptr<misa_json_schema_property> misaxx::parameter_registry::get_schema_builder() {
    return misa_runtime::instance().get_schema_builder();
}

std::shared_ptr<misa_json_schema_property>
parameter_registry::register_parameter(const std::vector<std::string> &t_path) {
    std::shared_ptr<misa_json_schema_property> current = get_schema_builder();
    for(const std::string &segment : t_path) {
        current = current->resolve(segment);
    }
    return current;
}
