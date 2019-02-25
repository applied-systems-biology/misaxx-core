//
// Created by ruman on 25.02.19.
//

#include <misaxx/core/json/misa_json_schema_property.h>

using namespace misaxx;

nlohmann::json misaxx::misa_json_schema_property::to_json() const {
    return nlohmann::json();
}

misaxx::misa_json_schema_property &misaxx::misa_json_schema_property::resolve(const std::string &key) {
    if(property_type != property_type::subtree)
        throw std::runtime_error("Cannot access schema subproperty of leaf property!");
    if(value_type != nlohmann::json::value_t::object)
        throw std::runtime_error("Cannot access schema subproperty of non-object property!");
    auto it = properties.find(key);
    if(it != properties.end())
        return *it->second;
    else {
        auto prop = std::make_shared<misa_json_schema_property>();
        properties[key] = prop;
        return *prop;
    }
}
