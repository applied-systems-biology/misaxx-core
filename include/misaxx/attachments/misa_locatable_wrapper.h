//
// Created by rgerst on 21.12.18.
//


#pragma once
#include <misaxx/attachments/misa_locatable.h>

namespace misaxx {
    /**
     * Wraps any misa_serializable value around a locatable
     * @tparam Value
     * @tparam Location
     */
    template<class Value, class Location> struct misa_locatable_wrapper : public misa_locatable<Location> {

        Value value;

        misa_locatable_wrapper() = default;

        explicit misa_locatable_wrapper(Value t_value, Location t_location) :
        misa_locatable<Location>(std::move(t_location)), value(std::move(t_value)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            misa_locatable<Location>::from_json(t_json);
            value = t_json["value"].template get<Value>();
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_locatable<Location>::to_json(t_json);
            t_json["value"] = value;
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            misa_locatable<Location>::to_json_schema(t_schema);
            value.to_json_schema(t_schema.resolve("value"));
        }

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_locatable<Location>::build_serialization_id_hierarchy(result);
            misa_serialization_id self = value.get_serialization_id();
            self.set_path(self.get_path() / "locatable");
            result.emplace_back(std::move(self));
        }

    };
}

namespace nlohmann {
    template<class Value, class Location>
    struct adl_serializer<misaxx::misa_locatable_wrapper<Value, Location>> {
        static void to_json(json &j, const misaxx::misa_locatable_wrapper<Value, Location> &opt) {
            opt.to_json(j);
        }

        static void from_json(const json &j, misaxx::misa_locatable_wrapper<Value, Location> &opt) {
            opt.from_json(j);
        }
    };
}