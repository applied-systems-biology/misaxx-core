//
// Created by rgerst on 12.12.18.
//

#pragma once
#include <array>
#include <boost/operators.hpp>
#include <misaxx/attachments/misa_quantity.h>

namespace misaxx {

    /**
    * Operators that apply to quantities
    */
    template<class Quantity, typename Value> using misa_matrix_operators_t =
    boost::equality_comparable<Quantity,
    boost::addable<Quantity,
    boost::addable2<Quantity, Value,
    boost::subtractable<Quantity,
    boost::subtractable2<Quantity, Value,
    boost::dividable2<Quantity, Value,
    boost::multipliable2<Quantity, Value,
    boost::incrementable<Quantity,
    boost::decrementable<Quantity>>>>>>>>>;

    struct misa_matrix_base {

    };

    /**
     * A matrix of misa_quantity
     * @tparam Value
     * @tparam Unit
     * @tparam Rows
     * @tparam Cols
     */
    template<typename Value, class Unit, size_t Rows, size_t Cols> struct misa_matrix :
            public misa_matrix_base,
            public misa_serializeable,
            private misa_matrix_operators_t <misa_matrix<Value, misa_unit_numeric, Rows, Cols>, Value> {

        using value_type = Value;
        using unit_type = Unit;
        static constexpr size_t rows = Rows;
        static constexpr size_t cols = Cols;

        misa_matrix() = default;

        misa_matrix(std::array<value_type, Rows * Cols> t_values, unit_type t_unit = unit_type()) : m_values(std::move(t_values)),
        m_unit(std::move(t_unit)) {

        }

        constexpr size_t size() const {
            return Rows * Cols;
        }

        value_type get_value(size_t row, size_t col) const {
            return m_values.at(col + row * cols);
        }

        value_type &get_value(size_t row, size_t col) {
            return m_values.at(col + row * cols);
        }

        unit_type get_unit() const {
            return m_unit;
        }

        /**
         * Returns a new matrix with the same unit, but casts the numeric values
         * @tparam T
         * @return
         */
        template<typename T> misa_matrix<T, Unit, Rows, Cols> cast_values() const {
            std::array<T, Rows * Cols> arr;
            for(size_t i = 0; i < arr.size(); ++i) {
                arr[i] = static_cast<T>(m_values[i]);
            }
            return misa_matrix<T, Unit, Rows, Cols>(std::move(arr), m_unit);
        }

        /**
        * Casts this matrix to a new unit
        * @param unit
        * @return
        */
        misa_matrix<Value, Unit, Rows, Cols> cast_unit(Unit unit) const {
            std::array<Value, Rows * Cols> arr;
            for(size_t i = 0; i < arr.size(); ++i) {
                arr[i] = Unit::convert(m_values[i], m_unit, unit);
            }
            return misa_matrix<Value, Unit, Rows, Cols>(std::move(arr), std::move(unit));
        }

        /**
       * Casts this matrix to a matrix without unit (numeric)
       * @param unit
       * @return
       */
        misa_matrix<Value, misa_unit_numeric, Rows, Cols> to_numeric() const {
            return misa_matrix<Value, misa_unit_numeric, Rows, Cols>(m_values);
        }

        bool operator==(const misa_matrix<Value, misa_unit_numeric, Rows, Cols> &rhs) const {
            return m_values == rhs.m_values && m_unit == rhs.m_unit;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator+=(const misa_matrix<Value, misa_unit_numeric, Rows, Cols> &rhs) {
            if (rhs.m_unit == m_unit) {
                for(size_t i = 0; i < size(); ++i) {
                    m_values[i] += rhs.m_values[i];
                }
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                *this += conv;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator-=(const misa_matrix<Value, misa_unit_numeric, Rows, Cols> &rhs) {
            if (rhs.m_unit == m_unit) {
                for(size_t i = 0; i < size(); ++i) {
                    m_values[i] -= rhs.m_values[i];
                }
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                *this -= conv;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator+=(const value_type &value) {
            for(auto& v : m_values) {
                v += value;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator-=(const value_type &value) {
            for(auto& v : m_values) {
                v -= value;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator*=(const value_type &value) {
            for(auto& v : m_values) {
                v *= value;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator/=(const value_type &value) {
            for(auto& v : m_values) {
                v /= value;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator%=(const value_type &value) {
            for(auto& v : m_values) {
                v %= value;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator++() {
            for(auto& v : m_values) {
                ++v;
            }
            return *this;
        }

        misa_matrix<Value, misa_unit_numeric, Rows, Cols> &operator--() {
            for(auto& v : m_values) {
                --v;
            }
            return *this;
        }

    private:
        std::array<Value, Rows * Cols> m_values;
        Unit m_unit;
    };

    /**
     * Convenience type for a matrix with 1 column
     */
    template<typename Value, class Unit, size_t Rows> using misa_vector = misa_matrix<Value, Unit, Rows, 1>;
}




