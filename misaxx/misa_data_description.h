//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * A description contains all necessary information to further process some data.
     * During importing, it is produced using a pattern.
     */
    struct misa_data_description : public misa_serializeable {
        void from_json(const nlohmann::json &t_json) override {
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return misa_serializeable::create_serialization_id_hierarchy(misa_serialization_id("misa", "descriptions/base"), {
                    misa_serializeable::get_serialization_id_hierarchy()
            });
        }
    };
}