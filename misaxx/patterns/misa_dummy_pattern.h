//
// Created by rgerst on 27.11.18.
//

#pragma once

#include <misaxx/misa_data_pattern.h>

namespace misaxx {
    /**
     * Special pattern that can be used as placeholder for manual linkage (without pattern).
     * @tparam T
     */
    template<typename T> struct misa_dummy_pattern : public misa_data_pattern<T> {
    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_data_pattern_base::build_serialization_id_hierarchy(result);
            result.emplace_back(misa_serialization_id("misa", "patterns/dummy"));
        }
    };
}

namespace nlohmann {
    template <class T> struct adl_serializer<misaxx::misa_dummy_pattern<T>> {
        static void to_json(json& j, const misaxx::misa_dummy_pattern<T>& value) {
            value.to_json(j);
        }

        static void from_json(const json& j, misaxx::misa_dummy_pattern<T>& value) {
            value.from_json(j);
        }
    };
}


