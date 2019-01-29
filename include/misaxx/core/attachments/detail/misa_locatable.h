//
// Created by rgerst on 29.01.19.
//


#pragma once

namespace misaxx {
    template<class Location>
    std::shared_ptr<const Location> misa_locatable::get_location() {
        if(static_cast<bool>(m_location)) {
            auto as = std::dynamic_pointer_cast<const Location>(m_location);
            if(static_cast<bool>(as))
                return as;
        }
        // Try to load from JSON
        if(!m_location_json.empty() && misa_serializeable::type_is_deserializeable_from_json<Location>(m_location_json)) {
            auto loc = std::make_shared<Location>();
            loc->from_json(m_location_json);
            m_location = loc;
            return m_location;
        }
        return std::shared_ptr<const Location>();
    }

    template<class Location>
    bool misa_locatable::has_location() {
        if(static_cast<bool>(m_location) && dynamic_cast<const Location*>(m_location.get()))
            return true;
        else
            return !m_location_json.empty() && misa_serializeable::type_is_deserializeable_from_json<Location>(m_location_json);
    }
}