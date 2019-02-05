//
// Created by rgerst on 05.02.19.
//

#pragma once
#include <misaxx/core/descriptions/misa_file_description.h>

namespace misaxx {
    struct misa_exported_attachments_description : public misa_file_description {
        using misa_file_description::misa_file_description;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_exported_attachments_description& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_exported_attachments_description& p) {
        p.from_json(j);
    }
}



