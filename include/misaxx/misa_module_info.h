//
// Created by rgerst on 16.01.19.
//

#pragma once

#include <string>
#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * Contains basic information about a MISA++ module
     */
    class misa_module_info : public misa_serializeable {
    public:
        misa_module_info() = default;

        misa_module_info(std::string t_name, std::string t_version);

        std::string get_name() const;
        std::string get_version() const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    private:
        std::string m_name;
        std::string m_version;
    };

    inline void to_json(nlohmann::json& j, const misa_module_info& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_module_info& p) {
        p.from_json(j);
    }
}



