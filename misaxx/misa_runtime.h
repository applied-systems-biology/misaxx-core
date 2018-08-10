//
// Created by rgerst on 10.08.18.
//


#pragma once

#include <pattxx/default_runtime.h>
#include "misa_module.h"

namespace misaxx {

    /**
     * Runtime based on pattxx::default_runtime that additionally contains all data necessary to run
     * a MISA++ module.
     */
    template<class Module> class misa_runtime : public pattxx::default_runtime<Module> {
    public:
        static_assert(std::is_base_of<misa_module_base, Module>::value, "Only MISA++ modules are supported!");
        using pattxx::default_runtime<Module>::default_runtime;
    };
}
