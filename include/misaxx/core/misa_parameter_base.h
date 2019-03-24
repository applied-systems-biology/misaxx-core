//
// Created by rgerst on 15.01.19.
//


#pragma once

#include <string>
#include <misaxx/core/misa_json_schema_property.h>

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
        * Location of the parameter within the parameter JSON
        */
        path location;

        /**
         * Parameter schema that belongs to the parameter
         */
        std::shared_ptr<misa_json_schema_property> schema;

        misa_parameter_base() = default;

        explicit misa_parameter_base(path t_location, std::shared_ptr<misa_json_schema_property> t_schema);

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

    };
}
