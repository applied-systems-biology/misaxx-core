//
// Created by ruman on 02.08.18.
//

#pragma once

#include "parameter_prepare.h"

/**
 * Contains the type for default preparation functions.
 * Template-specify the type to create custom default prepare actions.
 */
namespace misaxx::parameters {

    /**
     * Default operation: Do nothing
     * @tparam T
     */
    template<typename T> struct default_prepare_t : public prepare_base {
        static void prepare (const T &t_value) {
        }
    };

    /**
     * Template specialization that creates the parent path of the input
     */
    template<> struct default_prepare_t<path> : public prepare_base {
        static void prepare (const path &t_path) {
            boost::filesystem::create_directories(t_path.parent_path());
        }
    };
}

namespace misaxx::parameters {

    /**
     * Default operations to do when output is instantiated.
     * To add custom default prepare operations, template-specify misaxx::parameters::default_prepares::type
     */
    struct default_prepare {
        template<typename T> using type = default_prepare_t<T>;
    };

}