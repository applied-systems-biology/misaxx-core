//
// Created by rgerst on 11.12.18.
//

#include <misaxx/misa_serialization_id.h>

misaxx::misa_serialization_id::misa_serialization_id() : id("misa:unknown") {
}

misaxx::misa_serialization_id::misa_serialization_id(const std::string &t_module, const boost::filesystem::path &t_path)
        : id(t_module + ":" + t_path.string()) {

}

misaxx::misa_serialization_id::misa_serialization_id(std::string t_id) : id(std::move(t_id)) {
    auto colon_it = id.find(':');
    if(colon_it == std::string::npos)
        throw std::runtime_error("Invalid ID " + id);
}

std::string misaxx::misa_serialization_id::get_module() const {
    auto colon_it = id.find(':');
    if(colon_it == std::string::npos)
        throw std::runtime_error("Invalid ID " + id);
    return id.substr(0, colon_it);
}

boost::filesystem::path misaxx::misa_serialization_id::get_path() const {
    auto colon_it = id.find(':');
    if(colon_it == std::string::npos)
        throw std::runtime_error("Invalid ID " + id);
    return id.substr(colon_it + 1);
}

void misaxx::misa_serialization_id::set_module(const std::string &module) {
    auto p = get_path();
    set_id(module + ":" + p.string());
}

void misaxx::misa_serialization_id::set_path(const boost::filesystem::path &t_path) {
    auto m = get_module();
    set_id(m + ":" + t_path.string());
}

const std::string &misaxx::misa_serialization_id::get_id() const {
    return id;
}

void misaxx::misa_serialization_id::set_id(std::string t_id) {
    if(t_id.find(':') == std::string::npos)
        throw std::runtime_error("Invalid ID " + t_id);
    id = std::move(t_id);
}

bool misaxx::misa_serialization_id::empty() const {
    return id.empty();
}
