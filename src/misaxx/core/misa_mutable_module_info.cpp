//
// Created by rgerst on 17.01.19.
//

#include <misaxx/core/misa_mutable_module_info.h>

using namespace misaxx;

void misa_mutable_module_info::set_id(std::string t_id) {
    m_id = std::move(t_id);
}

void misa_mutable_module_info::set_name(std::string t_description) {
    m_description = std::move(t_description);
}

void misa_mutable_module_info::set_version(std::string t_version) {
    m_version = std::move(t_version);
}

void misa_mutable_module_info::add_dependency(misa_module_info t_dependency) {
    m_dependencies.emplace_back(std::move(t_dependency));
}

misa_mutable_module_info::operator misa_module_info() {
    return static_cast<misa_module_info>(*this);
}
