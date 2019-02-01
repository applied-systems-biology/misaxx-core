//
// Created by rgerst on 01.02.19.
//

#include "misa_runtime_log.h"
#include <chrono>

void misaxx::misa_runtime_log::start(uintptr_t id, std::string name) {
    std::lock_guard<std::mutex> lock {mutex};
    entry e {*this};
    e.start_time = clock::now();
    e.end_time = clock::now();
    e.name = std::move(name);
    entries[id] = std::move(e);
}

void misaxx::misa_runtime_log::stop(uintptr_t id) {
    std::lock_guard<std::mutex> lock {mutex};
    auto it = entries.find(id);
    if(it != entries.end()) {
        it->second.end_time = clock::now();
    }
}

void misaxx::misa_runtime_log::from_json(const nlohmann::json &) {
    throw std::runtime_error("Runtime logs cannot be loaded!");
}

void misaxx::misa_runtime_log::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    std::vector<entry> vec;
    for(const auto &kv : entries) {
        vec.push_back(kv.second);
    }
    t_json["entries"] = vec;
}

void misaxx::misa_runtime_log::to_json_schema(const misaxx::misa_json_schema &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("entries").declare_required<std::vector<entry>>();
}

void
misaxx::misa_runtime_log::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misaxx", "runtime-log"));
}

misaxx::misa_runtime_log::entry::entry(const misaxx::misa_runtime_log &log) : m_log(&log) {

}

void misaxx::misa_runtime_log::entry::from_json(const nlohmann::json &) {
    throw std::runtime_error("Runtime logs cannot be loaded!");
}

void misaxx::misa_runtime_log::entry::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    t_json["start-time"] = std::chrono::duration_cast<duration>(m_log->start_time - start_time).count();
    t_json["end-time"] = std::chrono::duration_cast<duration>(m_log->start_time - end_time).count();
    t_json["name"] = name;
}

void misaxx::misa_runtime_log::entry::to_json_schema(const misaxx::misa_json_schema &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("start-time").declare_required<double>();
    t_schema.resolve("end-time").declare_required<double>();
    t_schema.resolve("name").declare_required<std::string>();
}

void misaxx::misa_runtime_log::entry::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misaxx", "runtime-log/entry"));
}

