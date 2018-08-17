//
// Created by rgerst on 10.08.18.
//


#pragma once

namespace misaxx {
    template<class ModuleDefinition> struct misa_worker {
        using module_type = ModuleDefinition;
        virtual ModuleDefinition &module() = 0;
    };
}
