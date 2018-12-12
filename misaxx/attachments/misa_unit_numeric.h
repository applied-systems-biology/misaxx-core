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

        /**
         * This will always return misa_unit_numeric
         */
        template<size_t O> using select_order_type = misa_unit_numeric;

        misa_unit_numeric() = default;

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

        template<size_t O> misa_unit_numeric cast_order() const {
            return misa_unit_numeric();
        }

        /**
         * Allows intra-unit conversion
         * Automatically called by misa_quantity
         * @tparam T
         * @param t_value
         * @param t_src
         * @param t_dst
         * @return
         */
        template<typename T> static T convert(T t_value, const misa_unit_numeric &t_src, const misa_unit_numeric &t_dst) {
            return t_value;
        }
    };

    inline void to_json(nlohmann::json& j, const misa_unit_numeric& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_unit_numeric& p) {
        p.from_json(j);
    }
}



