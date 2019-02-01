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
    struct misa_locatable : public misa_serializable {

        misa_locatable() = default;

        explicit misa_locatable(std::shared_ptr<const misa_location> t_location);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(const misa_json_schema &t_schema) const override;

        /**
         * Returns the location as the specified class
         * @tparam Location
         * @return
         */
        template<class Location>
        std::shared_ptr<const Location> get_location();

        /**
         * Returns true if this locatable has the location
         * @tparam Location
         * @return
         */
        template<class Location>
        bool has_location();

        /**
         * Sets the location of this locatable
         * @param location
         */
        void set_location(std::shared_ptr<const misa_location> location);

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    private:
        /**
        * Pointer to the location.
        * The location is obtained from a cache (or created manually)
        */
        std::shared_ptr<const misa_location> m_location;
        /**
         * If deserialized from JSON
         */
        nlohmann::json m_location_json;
    };

    inline void to_json(nlohmann::json& j, const misa_locatable& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_locatable& p) {
        p.from_json(j);
    }
}

#include "detail/misa_locatable.h"
