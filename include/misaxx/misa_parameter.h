//
// Created by rgerst on 15.01.19.
//


#pragma once

#include <misaxx/json/misa_json_property.h>
#include <misaxx/runtime/misa_parameter_registry.h>
#include "misa_parameter_base.h"

namespace misaxx {
    /**
     * Allows access to a parameter
     * @tparam T
     */
    template<typename T> struct misa_parameter : public misa_parameter_base, public misa_json_property<T> {

        misa_parameter() = default;

        explicit misa_parameter(path t_location, misa_json_property<T> t_metadata = misa_json_property<T>()) :
                misa_json_property<T>(std::move(t_metadata)), location(std::move(t_location)) {

        }

        path location;

        const std::string &get_name() const override {
            return location[location.size() - 1];
        }

        const path &get_location() const override {
            return location;
        }

        const misa_json_property_base &get_description() const override {
            return *this;
        }

        /**
         * Gets the value of this parameter from the parameter file
         * @return
         */
        T query() const {
            return misaxx::parameter_registry::get_json(get_location(), *this);
        }
    };
}
