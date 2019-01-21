//
// Created by rgerst on 15.01.19.
//


#pragma once

#include <string>
#include <misaxx/core/json/misa_json_property_base.h>

namespace misaxx {
    /**
     * Base class of a parameter
     */
    struct misa_parameter_base {

        /**
         * Path type that is used for parameters
         */
        using path = std::vector<std::string>;

        /**
         * Returns the name of this parameter
         * @return
         */
        virtual const std::string &get_name() const = 0;

        /**
         * Returns the path of this parameter within the parameter file
         * @return
         */
        virtual const path &get_location() const = 0;

        /**
         * Returns the properties of this parameter, describing its contents
         * @return
         */
        virtual const misa_json_property_base &get_description() const = 0;
    };
}
