//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <misaxx/misa_module_declaration_base.h>
#include <misaxx/misa_submodule.h>

namespace misaxx {

    /**
     * The module definition contains which data is available inside the module, which data is exported and
     * which modules are included.
     * The module definition is combined with the module dispatcher (misaxx::misa_module) to build the
     * final module.
     */
    class misa_module_declaration : public misa_module_declaration_base {
    public:
        template<class Module> using submodule = misa_submodule<Module>;
    protected:

        /**
         * Initializes a submodule.
         * Please do not forget to setup the definition
         * @tparam Module
         * @param t_module
         * @param t_name
         */
        template<class Module> void init_submodule(submodule<Module> &t_module, const std::string &t_name) {
            t_module.name = t_name;
            t_module.definition().m_runtime = m_runtime;
            t_module.definition().filesystem = filesystem.create_subsystem(t_name);
            t_module.definition().init_data();
        }

    };
}
