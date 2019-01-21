//
// Created by rgerst on 07.11.18.
//

#include <misaxx/core/json/misa_json_schema.h>

misaxx::misa_json_schema::misa_json_schema(misaxx::misa_json_schema_builder &t_builder, std::vector<std::string> t_path)
        :
        m_builder(&t_builder), m_path(std::move(t_path)) {

}

misaxx::misa_json_schema_builder &misaxx::misa_json_schema::get_builder() const {
    return *m_builder;
}

misaxx::misa_json_schema misaxx::misa_json_schema::parent() const {
    if(m_path.empty())
        throw std::runtime_error("JSON schema is already parent!");
    std::vector<std::string> new_path = m_path;
    if(new_path.size() > 1) {
        std::swap(new_path[0], new_path[new_path.size() - 1]);
        new_path.erase(new_path.end() - 1);
    } else {
        new_path.clear();
    }
    return misa_json_schema(get_builder(), std::move(new_path));
}
