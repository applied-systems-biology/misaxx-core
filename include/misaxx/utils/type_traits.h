//
// Created by rgerst on 18.01.19.
//


#pragma once

namespace misaxx::utils {
    /**
     * Type trait that is always false regardless of input
     * @tparam ...
     */
    template <typename...> struct always_false { static constexpr bool value = false; };
}