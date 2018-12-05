//
// Created by rgerst on 05.12.18.
//

#pragma once

#include "misa_serialization_id.h"
#include "misa_serializeable.h"

namespace misaxx {
    /**
     * Type that allows renaming of attachments
     * This is intended to be used when data is attached to caches
     * @tparam Wrapped
     */
    template<class Wrapped> struct misa_renamed_attachment : public Wrapped {
        template<class ...Args> explicit misa_renamed_attachment(misa_serialization_id t_new_id, Args&& ...args) : m_new_id(std::move(t_new_id)),
        Wrapped(std::forward<Args>(args)...) {

        }

        misa_serialization_id get_serialization_id() const override {
            return m_new_id;
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            return misa_serializeable::create_serialization_id_hierarchy(*this, { Wrapped::get_serialization_id_hierarchy() });
        }

    private:
        misa_serialization_id m_new_id;
    };
}




