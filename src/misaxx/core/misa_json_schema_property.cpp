//
// Created by ruman on 25.02.19.
//

#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

std::shared_ptr<misaxx::misa_json_schema_property> misaxx::misa_json_schema_property::resolve(const std::string &key) {
    if(property_type != property_type::subtree)
        throw std::runtime_error("Cannot access schema subproperty of leaf property!");
    if(value_type != nlohmann::json::value_t::object)
        throw std::runtime_error("Cannot access schema subproperty of non-object property!");
    auto it = properties.find(key);
    if(it != properties.end())
        return it->second;
    else {
        auto prop = std::make_shared<misa_json_schema_property>();
        properties[key] = prop;
        return prop;
    }
}

std::shared_ptr<misa_json_schema_property> misa_json_schema_property::self() {
    return shared_from_this();
}

std::shared_ptr<const misa_json_schema_property> misa_json_schema_property::self() const {
    return shared_from_this();
}

void misa_json_schema_property::to_json(nlohmann::json &json) const {

}

std::shared_ptr<misa_json_schema_property> misa_json_schema_property::resolve(const std::vector<std::string> &keys) {
    std::shared_ptr<misa_json_schema_property> result = self();
    for(const std::string &key : keys) {
        result = result->resolve(key);
    }
    return result;
}
