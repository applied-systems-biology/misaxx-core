//
// Created by rgerst on 17.01.19.
//

#include <misaxx/core/misa_mutable_module_info.h>

using namespace misaxx;

void misa_mutable_module_info::set_id(std::string t_id) {
    m_id = std::move(t_id);
}

void misa_mutable_module_info::set_name(std::string t_description) {
    m_name = std::move(t_description);
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

void misa_mutable_module_info::set_description(std::string t_description) {
    m_description = std::move(t_description);
}

void misa_mutable_module_info::set_url(std::string t_url) {
    m_url = std::move(t_url);
}

void misa_mutable_module_info::add_author(std::string t_author) {
    m_authors.emplace_back(std::move(t_author));
}

void misa_mutable_module_info::set_authors(std::vector<std::string> t_authors) {
    m_authors = std::move(t_authors);
}

void misa_mutable_module_info::set_organization(std::string t_organization) {
    m_organization = std::move(t_organization);
}

void misa_mutable_module_info::set_license(std::string t_license) {
    m_license = std::move(t_license);
}

void misa_mutable_module_info::set_citation(std::string t_citation) {
    m_citation = std::move(t_citation);
}

void misa_mutable_module_info::set_is_external(bool t_flag) {
    m_is_external = t_flag;
}
