//
// Created by rgerst on 30.07.18.
//


#pragma once

#include "parameter_checks.h"

namespace misaxx::parameters {

    /**
     * Base type for all output prepare classes
     */
    struct prepare_base {

    };

    /**
     * Do no output preparation
     */
    struct no_prepare {
        /**
        * Default oeration: Do nothing
        * @tparam T
        */
        template<typename T> struct type : public prepare_base{
            static void prepare (const T &t_value) {
            }
        };
    };
}