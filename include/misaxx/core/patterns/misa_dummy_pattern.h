//
// Created by rgerst on 27.11.18.
//

#pragma once

#include <misaxx/core/misa_data_pattern.h>

namespace misaxx {
    /**
     * Special pattern that can be used as placeholder for manual linkage (without pattern).
     */
    struct misa_dummy_pattern : public misa_data_pattern {

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_dummy_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_dummy_pattern& p) {
        p.from_json(j);
    }
}
