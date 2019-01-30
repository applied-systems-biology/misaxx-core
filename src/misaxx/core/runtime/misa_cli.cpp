#include <misaxx/core/runtime/misa_cli.h>
#include "misa_runtime.h"

std::shared_ptr<misaxx::misa_runtime> misaxx::create_runtime(misaxx::misa_module_info t_module_info,
                                                             std::shared_ptr<misaxx::misa_module_interface> t_module_interface_builder,
                                                             std::function<std::shared_ptr<misaxx::misa_dispatcher>(
                                                                     const std::shared_ptr<misaxx::misa_work_node> &,
                                                                     std::shared_ptr<misaxx::misa_module_interface>)> t_module_dispatcher_builder) {
    auto result = std::make_shared<misaxx::misa_runtime>(std::move(t_module_info), std::move(t_module_interface_builder), std::move(t_module_dispatcher_builder));
    misaxx::misa_runtime::singleton_instance = result;
    return result;
}
