//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <nlohmann/json.hpp>
#include <misaxx/json/misa_json_schema.h>
#include <misaxx/misa_serialization_id.h>

namespace misaxx {
    /**
     * An object that can be serialized and deserialized from/to JSON
     *
     */
    struct misa_serializeable {
        /**
         * Deserializes the JSON data
         * @param t_json
         */
        virtual void from_json(const nlohmann::json &t_json) = 0;

        /**
         * Serializes the JSON data
         * @param t_json
         */
        virtual void to_json(nlohmann::json &t_json) const {
            t_json["misa:serialization-hierarchy"] = get_serialization_id_hierarchy();
            t_json["misa:serialization-id"] = get_serialization_id().get_id();
        }

        /**
         * Describes the structure of the data as JSON schema
         * Must be consistent with the JSON structure expected by from_json and to_json
         * @param t_schema
         */
        virtual void to_json_schema(const misa_json_schema &t_schema) const = 0;

        /**
         * Gets the serialization ID of the object
         * @return
         */
        misa_serialization_id get_serialization_id() const {
            const auto h = get_serialization_id_hierarchy();
            return h[h.size() - 1];
        }

        /**
         * Gets a hierarchy of serialization IDs that is consistent with the inheritance hierarchy.
         * The hierarchy should include the current serialization id and should be ordered such as the
         * current id is the last entry
         * @return
         */
        virtual std::vector<misa_serialization_id> get_serialization_id_hierarchy() const {
            return { misa_serialization_id("misa", "serializeable") };
        }
        /**
         * Creates a serialization id hierarchy from the current object and the inherited serialization ids         *
         * @param self
         * @param bases
         * @return
         */
        static std::vector<misa_serialization_id> create_serialization_id_hierarchy(misa_serialization_id self,
                const std::vector<std::vector<misa_serialization_id>> &bases);

        /**
         * Deserializes a wrapped value from the JSON
         * @tparam T
         * @param t_json
         * @return
         */
        template<typename T> static T deserialize_wrapped(const nlohmann::json &t_json) {
            if(t_json.is_object() && t_json.find("misa:serializeable/value") != t_json.end()) {
                return t_json["misa:serializeable/value"].get<T>();
            }
            else {
                t_json.get<T>();
            }
        }

        /**
         * Serializes objects as-is, but other JSON types wrapped into an misa:serializeable/value attibute.
         * This is required, so misa_serializeable can attach type information
         * @tparam T
         * @param value
         * @param t_json
         */
        template<typename T> static void serialize_wrapped(const T &value, nlohmann::json &t_json) {
            nlohmann::json v = value;
            if(v.is_object()) {
                for(auto it = v.begin(); it != v.end(); ++it) {
                    t_json[it.key()] = it.value();
                }
            }
            else {
                t_json["misa:serializeable/value"] = std::move(v);
            }
        }
    };
}
