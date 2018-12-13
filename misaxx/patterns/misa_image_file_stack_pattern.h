//
// Created by rgerst on 02.11.18.
//


#pragma once

#include <misaxx/patterns/misa_file_pattern.h>
#include <misaxx/patterns/misa_file_stack_pattern.h>

namespace misaxx {
    /**
     * File pattern with predefined extensions that cover common image formats
     */
    struct misa_image_file_stack_pattern : public misa_file_stack_pattern {

        misa_image_file_stack_pattern();

        void from_json(const nlohmann::json &t_json) override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_image_file_stack_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_image_file_stack_pattern& p) {
        p.from_json(j);
    }
}
