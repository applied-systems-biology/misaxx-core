//
// Created by rgerst on 11.12.18.
//

#pragma once

#include <cxxh/string.h>
#include "misa_unit.h"

namespace misaxx {

    /**
     * Special unit that multiplies and divides into itself
     */
    struct misa_unit_numeric : public misa_unit<1> {

        using higher_order_type = misa_unit_numeric;
        using lower_order_type = misa_unit_numeric;

        void from_json(const nlohmann::json &t_json) override {
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            misa_serialization_id self("misa", "attachments/quantities/numeric");
            return misa_serializeable::create_serialization_id_hierarchy(std::move(self), {
                    misa_unit<1>::get_serialization_id_hierarchy()
            });
        }

        std::string get_literal() const override {
            return ""; // No literal
        }

        bool operator==(const misa_unit_numeric &rhs) const {
            return true;
        }

        bool operator!=(const misa_unit_numeric &rhs) const {
            return false;
        }
    };

    inline void to_json(nlohmann::json& j, const misa_unit_numeric& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_unit_numeric& p) {
        p.from_json(j);
    }
}



