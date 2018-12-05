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
        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "patterns/dummy");
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return create_serialization_id_hierarchy(*this, { misa_data_pattern<T>::get_serialization_id_hierarchy() });
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


