//
// Created by rgerst on 21.09.18.
//


#pragma once

#include <string>
#include "misa_submodule.h"
#include "misa_module_base.h"

namespace misaxx {
    /**
     * Contains instructions for a future dispatch operation.
     * Required to later build the standalone module.
     * @tparam Instance
     */
    template<class Instance> struct misa_future_dispatch {
        using instance_type = Instance;
        std::string name;

        explicit misa_future_dispatch(std::string t_name) : name(std::move(t_name)) {
        }
    };
}
