//
// Created by rgerst on 21.12.18.
//


#pragma once

#include <type_traits>
#include <misaxx/core/attachments/misa_location.h>

namespace misaxx {

    /**
     * Base type for all attachments that can be located
     * @tparam Location
     */
    template<class Location> struct misa_locatable : public misa_serializeable {
        static_assert(std::is_base_of<misa_location, Location>::value, "Unsupported location type!");

        /**
         * Location of this locatable attachment
         */
        Location location;

        misa_locatable() = default;

        explicit misa_locatable(Location t_location) : location(std::move(t_location)) {

        }

        void from_json(const nlohmann::json &t_json) override {
            location = t_json["location"].template get<Location>();
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            t_json["location"] = location;
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            misa_serializeable::to_json_schema(t_schema);
            location.to_json_schema(t_schema.resolve("location"));
        }

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_serializeable::build_serialization_id_hierarchy(result);
            misa_serialization_id self = location.get_serialization_id();
            self.set_path(self.get_path() / "locatable");
            result.emplace_back(std::move(self));
        }
    };
}

namespace nlohmann {
    template<class Location>
    struct adl_serializer<misaxx::misa_locatable<Location>> {
        static void to_json(json &j, const misaxx::misa_locatable<Location> &opt) {
            opt.to_json(j);
        }

        static void from_json(const json &j, misaxx::misa_locatable<Location> &opt) {
            opt.from_json(j);
        }
    };
}

