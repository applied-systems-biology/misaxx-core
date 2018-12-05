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
        virtual void to_json(nlohmann::json &t_json) const = 0;

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
        virtual misa_serialization_id get_serialization_id() const = 0;

        /**
         * Gets a hierarchy of serialization IDs that is consistent with the inheritance hierarchy.
         * The hierarchy should include the current serialization id and should be ordered such as the
         * current id is the last entry
         * @return
         */
        virtual std::vector<misa_serialization_id> get_serialization_id_hierarchy() const = 0;

        /**
         * Creates a serialization id hierarchy from the current object and the inherited serialization ids         *
         * @param self
         * @param bases
         * @return
         */
        static std::vector<misa_serialization_id> create_serialization_id_hierarchy(const misa_serializeable &self,
                const std::vector<std::vector<misa_serialization_id>> &bases) {
            std::vector<misa_serialization_id> result;
            result.push_back(self.get_serialization_id());
            for(const auto &v : bases) {
                std::copy(v.begin(), v.end(), std::back_inserter(result));
            }
            return result;
        }
    };
}
