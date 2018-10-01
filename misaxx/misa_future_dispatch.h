//
// Created by rgerst on 26.09.18.
//


#pragma once

#include <string>

namespace misaxx {
    template<class Instance> struct misa_future_dispatch {
        using instance_type = Instance;
        std::string name;
        explicit misa_future_dispatch(std::string t_name) : name(std::move(t_name)) {

        }
    };
}
