//
// Created by rgerst on 11.12.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>

namespace misaxx {
    /**
     * Base class for a unit supported by the MISA++ math types.
     * A unit can be one specific unit (e.g. unit_millimeters) or a group of
     * related and convertible units (e.g. unit_length) where the mode is stored inside of
     * the object.
     *
     * A unit must have a typedef 'select_order_type<size_t Order>' that returns the unit for given order
     * order 0 or lower should be misa_unit_numeric
     *
     * Additionally, a unit must have a static function convert(T src, unit src_unit, unit dst_unit)
     * that converts between the internal unit modes
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

    /**
     * Selects the higher order of given unit
     */
    template<class Unit> using misa_unit_higher_order = typename Unit::template select_order_type<Unit::order + 1>;

    /**
     * Selects the higher order of given unit
     * If the order of Unit is 1, it will decay to misa_unit_numeric
     */
    template<class Unit> using misa_unit_lower_order = typename Unit::template select_order_type<Unit::order - 1>;
}




