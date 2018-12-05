//
// Created by rgerst on 05.12.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {

    /**
     * A range of values
     * @tparam T
     */
    template<typename T> struct misa_range : public misa_serializeable {

        T from = T();
        T to = T();

        misa_range() = default;

        misa_range(T t_from, T t_to) : from(std::move(t_from)), to(std::move(t_to)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            from = t_json["from"].get<T>();
            to = t_json["to"].get<T>();
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            t_json["from"] = from;
            t_json["to"] = to;
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("from").declare_required<T>();
            t_schema.resolve("to").declare_required<T>();
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", std::string("attachments/range/") + typeid(T).name());
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return { get_serialization_id() };
        }
    };
}

namespace nlohmann {
    template <typename T>
    struct adl_serializer<misaxx::misa_range<T>> {
        static void to_json(json& j, const misaxx::misa_range<T> &v) {
           v.to_json(j);
        }

        static void from_json(const json& j, misaxx::misa_range<T>&v) {
            v.from_json(j);
        }
    };
}



