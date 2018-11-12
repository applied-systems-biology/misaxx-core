//
// Created by rgerst on 02.11.18.
//


#pragma once

#include "misa_file_description.h"

namespace misaxx {
    struct misa_image_file_description : public misa_file_description {
        using misa_file_description::misa_file_description;

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "descriptions/image-file");
        }

    };

    void to_json(nlohmann::json& j, const misa_image_file_description& p) {
        p.to_json(j);
    }

    void from_json(const nlohmann::json& j, misa_image_file_description& p) {
        p.from_json(j);
    }
}
