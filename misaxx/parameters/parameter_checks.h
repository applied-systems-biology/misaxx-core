//
// Created by rgerst on 30.07.18.
//


#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace misaxx::parameters {

    /**
     * Base class for all checks.
     * Requires for static checks
     */
    struct check_base {

    };

    /**
     * Path type supported in this library.
     */
    using path = boost::filesystem::path;

    /**
    * Do no check
    */
    struct no_check {
        template<typename T> struct type : public check_base {
            static bool check (const T &t_value) {
                return true;
            }
        };
    };
}