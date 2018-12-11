//
// Created by rgerst on 11.12.18.
//

#pragma once

#include <misaxx/misa_serializeable.h>
#include <misaxx/attachments/misa_unit_numeric.h>
#include <boost/operators.hpp>
#include <ostream>

namespace misaxx {

    /**
     * Operators that apply to quantities
     */
    template<class Quantity, typename Value> using misa_quantity_operators_t =
    boost::partially_ordered<Quantity,
    boost::equality_comparable<Quantity,
    boost::addable<Quantity,
    boost::subtractable<Quantity,
    boost::dividable2<Quantity, Value,
    boost::multipliable2<Quantity, Value,
    boost::incrementable<Quantity,
    boost::decrementable<Quantity>>>>>>>>;

    template<typename Value, class Unit>
    struct misa_quantity :
            public misa_serializeable,
            private misa_quantity_operators_t <misa_quantity<Value, Unit>, Value>{

        using value_type = Value;
        using unit_type = Unit;

        misa_quantity() = default;

        misa_quantity(value_type t_value, unit_type t_unit = unit_type()) :

        m_value(std::move(t_value)), m_unit(std::move(t_unit)) {

        }

        /**
         * Casts the quantity to the numeric value
         * @return
         */
        explicit operator value_type() const {
            return get_value();
        }

        /**
         * Returns a new quantity with the same unit, but casts the numeric value
         * @tparam T
         * @return
         */
        template<typename T> explicit operator misa_quantity<T, Unit>() {
            return cast_value<T>();
        }

        value_type get_value() const {
            return m_value;
        }

        value_type &get_value() {
            return m_value;
        }

        unit_type get_unit() const {
            return m_unit;
        }

        template<typename T> misa_quantity<T, Unit> cast_value() {
            return misa_quantity<T, Unit>(static_cast<T>(m_value), m_unit);
        }

        void from_json(const nlohmann::json &t_json) override {
            m_value = t_json["value"].get<Value>();
            m_unit.from_json(t_json["unit"]);
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializeable::to_json(t_json);
            t_json["value"] = m_value;
            m_unit.to_json(t_json["unit"]);
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.resolve("value").declare_required<Value>();
            t_schema.resolve("unit").declare_required<Unit>();
        }

        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const override {
            misa_serialization_id self = m_unit.get_serialization_id();
            self.set_path(self.get_path() / std::string(typeid(Value).name()));
            return misa_serializeable::create_serialization_id_hierarchy(std::move(self), {
                    misa_serializeable::get_serialization_id_hierarchy()
            });
        }

        bool operator==(const misa_quantity<Value, Unit> &rhs) const {
            return m_value == rhs.m_value && m_unit == rhs.m_unit;
        }

        bool operator<(const misa_quantity<Value, Unit> &rhs) const {
            if (rhs.m_unit == m_unit) {
                return m_value < rhs.m_value;
            } else {
                const auto conv = this->as(rhs.m_unit);
                return conv < rhs;
            }
        }

        misa_quantity<Value, Unit> &operator+=(const misa_quantity<Value, Unit> &rhs) {
            if (rhs.m_unit == m_unit) {
                m_value += rhs.m_value;
            } else {
                const auto conv = rhs.as(m_unit);
                *this += conv;
            }
        }

        misa_quantity<Value, Unit> &operator-=(const misa_quantity<Value, Unit> &rhs) {
            if (rhs.m_unit == m_unit) {
                m_value -= rhs.m_value;
            } else {
                const auto conv = rhs.as(m_unit);
                *this -= conv;
            }
        }

        misa_quantity<Value, Unit> &operator*=(const value_type &value) {
            this->m_value *= value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator/=(const value_type &value) {
            this->m_value /= value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator%=(const value_type &value) {
            this->m_value %= value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator++() {
            ++this->m_value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator--() {
            --this->m_value;
            return *this;
        }

        /**
         * Multiplication that will increase the order of the result
         * @param lhs
         * @param rhs
         * @return
         */
        friend misa_quantity<Value, typename Unit::higher_order_type> operator *(const misa_quantity<Value, Unit> &lhs, const misa_quantity<Value, Unit> &rhs) {
            misa_quantity<Value, typename Unit::higher_order_type> result;
            Value r = Unit::convert(rhs.m_value, rhs.m_unit, lhs.m_unit); // Convert rhs to the unit of lhs
            result.m_value = lhs.m_value * r;
            return result;
        }

        /**
         * Division that will decrease the order of the result
         * @param lhs
         * @param rhs
         * @return
         */
        friend misa_quantity<Value, typename Unit::lower_order_type> operator /(const misa_quantity<Value, Unit> &lhs, const misa_quantity<Value, Unit> &rhs) {
            misa_quantity<Value, typename Unit::lower_order_type> result;
            Value r = Unit::convert(rhs.m_value, rhs.m_unit, lhs.m_unit); // Convert rhs to the unit of lhs
            result.m_value = lhs.m_value / r;
            return result;
        }

        friend std::ostream &operator<<(std::ostream &os, const misa_quantity &quantity) {
            os << cxxh::to_string(quantity.m_value) << quantity.m_unit.get_literal();
            return os;
        }

    private:
        Value m_value{};
        Unit m_unit{};
    };
}

namespace nlohmann {
    template<typename Value, class Unit>
    struct adl_serializer<misaxx::misa_quantity<Value, Unit>> {
        static void to_json(json &j, const misaxx::misa_quantity<Value, Unit> &opt) {
            opt.to_json(j);
        }

        static void from_json(const json &j, misaxx::misa_quantity<Value, Unit> &opt) {
            opt.from_json(j);
        }
    };
}

