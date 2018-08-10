//
// Created by rgerst on 10.08.18.
//


#pragma once
#include <memory>

namespace misaxx {
    /**
     * The module definition contains which data is available inside the module, which data is exported and
     * which modules are included.
     * The module definition is combined with the module dispatcher (misaxx::misa_module) to build the
     * final module.
     */
    class misa_module_definition {

        template<class Module> using submodule = Module*;

    protected:

        template<class Data, typename ... Args> Data declare_data(Args&&... arguments) {
           Data value(std::forward(arguments)...);
           return value;
        }

        template<class Module> submodule<Module> import_module() {
            return nullptr;
        }

    };
}
