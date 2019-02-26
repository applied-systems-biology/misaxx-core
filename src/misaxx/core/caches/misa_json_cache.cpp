//
// Created by rgerst on 26.02.19.
//

#include <misaxx/core/caches/misa_json_cache.h>
#include <iomanip>

#include "misaxx/core/caches/misa_json_cache.h"

void misaxx::misa_json_cache::simulate_link() {
    misa_default_cache::simulate_link();
}

void misaxx::misa_json_cache::do_link(const misaxx::misa_json_description &t_description) {
    m_filename = get_location() / t_description.filename;
    set_unique_location(m_filename);
}

misaxx::misa_json_description misaxx::misa_json_cache::produce_description(const boost::filesystem::path &t_location,
                                                                           const misaxx::misa_json_pattern &t_pattern) {
    misa_json_description result;
    t_pattern.apply(result, t_location);
    return result;
}

const boost::filesystem::path &misaxx::misa_json_cache::get_filename() const {
    return m_filename;
}

nlohmann::json &misaxx::misa_json_cache::get() {
    return m_data;
}

const nlohmann::json &misaxx::misa_json_cache::get() const {
    return m_data;
}

void misaxx::misa_json_cache::set(nlohmann::json value) {
    m_data = std::move(value);
}

bool misaxx::misa_json_cache::has() const {
    return m_loaded;
}

bool misaxx::misa_json_cache::can_pull() const {
    return boost::filesystem::exists(m_filename);
}

void misaxx::misa_json_cache::pull() {
    std::ifstream stream;
    stream.open(m_filename.string());
    stream >> m_data;
}

void misaxx::misa_json_cache::stash() {
    m_data = nlohmann::json();
}

void misaxx::misa_json_cache::push() {
    std::ofstream stream;
    stream.open(m_filename.string());
    stream << std::setw(4) << m_data;
}
