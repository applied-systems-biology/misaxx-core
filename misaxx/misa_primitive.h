//
// Created by rgerst on 09.11.18.
//


#pragma once

#include "misa_serializeable.h"

namespace misaxx {
    /**
     * A wrapper around a primitive value that supports dynamic serialization (misa_serializable)
     * @tparam T Must be compile-type serializeable by nlohmann::json
     */
    template<typename T> struct misa_primitive : public misa_serializeable {
        T value = T();
        misa_json_property<T> metadata;

        misa_primitive() = default;

        misa_primitive(T v) : value(std::move(v)) {

        }

        operator T() {
            return value;
        }

        void from_json(const nlohmann::json &t_json) override {
            value = t_json;
        }

        void to_json(nlohmann::json &t_json) const override {
            t_json = nlohmann::json(value);
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.declare(metadata);
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", std::string("primitive/") + typeid(T).name());
        }
    };

    /**
     * Returns a misa_primitive<T> if needed to wrap the value
     */
    template<typename T> using misa_serializeable_value = typename std::conditional<std::is_base_of<misa_serializeable, T>::value, T, misa_primitive<T>>::type;
}
