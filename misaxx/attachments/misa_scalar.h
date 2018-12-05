//
// Created by rgerst on 05.12.18.
//

#pragma once


#include <misaxx/misa_primitive.h>

namespace misaxx {

    /**
     * A scalar value that supports calculation and relational operators, but can also be serialized
     * @tparam T
     */
    template<typename T> struct misa_scalar : public misa_primitive<T> {
        using misa_primitive<T>::misa_primitive;

        bool operator==(const misa_scalar &rhs) const {
            return this->value == rhs.value;
        }

        bool operator!=(const misa_scalar &rhs) const {
            return !(rhs == *this);
        }

        bool operator<(const misa_scalar &rhs) const {
            return this->value < rhs.value;
        }

        bool operator>(const misa_scalar &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const misa_scalar &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const misa_scalar &rhs) const {
            return !(*this < rhs);
        }

        misa_scalar<T>& operator+=(const misa_scalar<T>& rhs) {
            this->value += rhs.value;
            return *this;
        }

        friend misa_scalar<T> operator+(misa_scalar<T> lhs,
                           const misa_scalar<T>& rhs)
        {
            lhs += rhs;
            return lhs;
        }

        misa_scalar<T>& operator-=(const misa_scalar<T>& rhs) {
            this->value -= rhs.value;
            return *this;
        }

        friend misa_scalar<T> operator-(misa_scalar<T> lhs,
                                        const misa_scalar<T>& rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        misa_scalar<T>& operator*=(const misa_scalar<T>& rhs) {
            this->value *= rhs.value;
            return *this;
        }

        friend misa_scalar<T> operator*(misa_scalar<T> lhs,
                                        const misa_scalar<T>& rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        misa_scalar<T>& operator/=(const misa_scalar<T>& rhs) {
            this->value /= rhs.value;
            return *this;
        }

        friend misa_scalar<T> operator/(misa_scalar<T> lhs,
                                        const misa_scalar<T>& rhs)
        {
            lhs /= rhs;
            return lhs;
        }

        misa_scalar<T>& operator++()
        {
            // actual increment takes place here
            return *this;
        }

        misa_scalar<T> operator++(int)
        {
            misa_scalar<T> tmp(*this); // copy
            operator++(); // pre-increment
            return tmp;   // return old value
        }
    };

}



