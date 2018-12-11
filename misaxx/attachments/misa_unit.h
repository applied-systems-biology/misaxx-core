//
// Created by rgerst on 11.12.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * Base class for a unit supported by the MISA++ math types
     *
     * A unit must have two typedefs higher_order_type, which is the type with 1 order higher
     * and lower_order_type, which reduces the order.
     * lower_order_type for an unit with order 1 should be misa_unit_numeric
     *
     * @tparam Order Order of the unit. Must be at least 1
     */
    template<size_t Order> struct misa_unit : public misa_serializeable {
        static constexpr size_t order = Order;
        static_assert(Order >= 1, "The order must be at least 1");

        /**
         * Returns a literal that represents the unit (e.g. mm^2)
         * @return
         */
        virtual std::string get_literal() const = 0;
    };
}




