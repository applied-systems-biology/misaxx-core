//
// Created by ruman on 02.08.18.
//

#pragma once

#include "parameter_checks.h"

/**
 * Contains the actual types for the parameter_default_check
 * Template-specialize for your type inside this namespace to create custom default checks.
 */
namespace misaxx::parameters {

    /**
     * Default check type. Returns true to every input.
     * @tparam T
     */
    template<typename T>
    struct default_check_t : public check_base {
        static bool check(const T &t_value) {
            return true;
        }
    };

    /**
     * Template specialization that checks if a path exists.
     */
    template<>
    struct default_check_t<path> : public check_base {
        static bool check(const path &t_path) {
            return boost::filesystem::exists(t_path);
        }
    };

    /**
     * Template specialization for a vector of items
     * @tparam U
     * @tparam Void
     */
    template<class U>
    struct default_check_t<std::vector<U>> : public check_base {
        using vector_type = std::vector<U>;
        using value_type = typename vector_type::value_type;
        using item_check_type = default_check_t<U>;

        static bool check(const vector_type &t_vec) {
            auto f = item_check_type();

            for (const auto &i : t_vec) {
                if (!f(i)) {
                    return false;
                }
            }

            return true;
        }
    };

}

namespace misaxx::parameters {

    /**
    * Included check check.
    * This type is a tag-type. The actual check is contained within default_check::type
    * Also supports vectors of items and checks them individually using the correct default_check function
    * To create custom default checks, template-specify misaxx::parameters::default_checks::type
    */
    struct default_check {
        template<typename T> using type = default_check_t<T>;
    };

}
