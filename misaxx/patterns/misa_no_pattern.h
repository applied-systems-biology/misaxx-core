//
// Created by rgerst on 27.11.18.
//

#pragma once

#include <misaxx/misa_data_pattern.h>

namespace misaxx {
    /**
     * Special pattern that can be used as placeholder for manual linkage (without pattern).
     * @tparam T
     */
    template<typename T> struct misa_no_pattern : public misa_data_pattern<T> {
    };
}




