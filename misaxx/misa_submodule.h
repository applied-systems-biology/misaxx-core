//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <misaxx/filesystem/misa_filesystem.h>
#include <misaxx/misa_module_declaration_base.h>

namespace misaxx {

    struct misa_module_declaration_base;

    struct misa_submodule_base {
    };

    /**
     * Wraps around a module that can be dispatched later.
     * @tparam Module
     */
    template<class Module, class ModuleDeclaration = typename Module::module_type>
    class misa_submodule : public misa_submodule_base {
    public:

        Module *m_module = nullptr;
        std::string name;

        using module_type = Module;
        using module_declaration_type = ModuleDeclaration;

        /**
         * The module instance if available.
         * @return
         */
        Module &instance() {
            return *m_module;
        }

        /**
         * Returns the current applicable module definition (the preset definition if not dispatched
         * or the module instance if already dispatched).
         * @return
         */
        module_declaration_type &definition() {
            if (m_module != nullptr)
                return *m_module;
            else
                return m_module_declaration;
        }

        misa_filesystem &get_filesystem() {
            return m_module_declaration.filesystem;
        }

        bool has_instance() const {
            return m_module != nullptr;
        }

    private:
        module_declaration_type m_module_declaration;
    };
}
