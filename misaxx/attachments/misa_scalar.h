//
// Created by rgerst on 05.12.18.
//

#pragma once


#include <misaxx/misa_primitive.h>
#include <boost/operators.hpp>

namespace misaxx {

    /**
     * Helper that adds boost operators to the target
     * @tparam Target
     * @tparam T
     */
    template<class Target, typename T> struct scalar_operators : public boost::partially_ordered<Target,
                                                     boost::equality_comparable<Target,
                                                     boost::addable<Target,
                                                     boost::subtractable<Target,
                                                     boost::dividable<Target, T,
                                                     boost::multipliable2<Target, T,
                                                     boost::incrementable<Target,
                                                     boost::decrementable<Target>>>>>>>> {
        
    };

    /**
     * A scalar value that supports calculation and relational operators, but can also be serialized
     * @tparam T
     */
    template<typename T> struct misa_scalar : public misa_primitive<T>, public scalar_operators<misa_scalar<T>, T> {
        using misa_primitive<T>::misa_primitive;

        bool operator==(const misa_scalar<T> &rhs) const {
            return this->value == rhs.value;
        }

        bool operator<(const misa_scalar<T> &rhs) const {
            return this->value < rhs.value;
        }

        misa_scalar<T>& operator+=(const misa_scalar<T>& rhs) {
            this->value += rhs.value;
            return *this;
        }

        misa_scalar<T>& operator-=(const misa_scalar<T>& rhs) {
            this->value -= rhs.value;
            return *this;
        }

        misa_scalar<T>& operator*=(const misa_scalar<T>& rhs) {
            this->value *= rhs.value;
            return *this;
        }

        misa_scalar<T>& operator/=(const misa_scalar<T>& rhs) {
            this->value /= rhs.value;
            return *this;
        }
    };

}



