//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "misa_filesystem.h"
#include "misa_module_declaration_base.h"
#include "misa_module_data.h"

namespace misaxx {

    struct misa_module_declaration_base;

    struct misa_submodule_base : public misa_module_data {
        using misa_module_data::misa_module_data;
    };

    /**
     * Wraps around a module that can be dispatched later.
     * @tparam Module
     */
    template<class Module, class ModuleDefinition = typename Module::module_type>
    class misa_submodule : public misa_submodule_base {
    public:

        Module *m_module = nullptr;

        using module_type = Module;
        using module_definition_type = ModuleDefinition;

        explicit misa_submodule(misa_module_declaration_base &t_parent, std::string t_name, pattxx::metadata t_metadata) :
                misa_submodule_base(t_parent, std::move(t_name), std::move(t_metadata)) {
        }

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
        module_definition_type &definition() {
            if (m_module != nullptr)
                return *m_module;
            else
                return m_module_definition;
        }

        misa_filesystem &get_filesystem() {
            return m_module_definition.filesystem;
        }

        bool has_instance() const {
            return m_module != nullptr;
        }

        /**
         * A submodule cannot be cleared
         */
        void clear() override {
            throw std::runtime_error("A submodule cannot be cleared!");
        }

        /**
         * Initializes this submodule definition
         * Call this method inside misa_module::init()
         * @param module
         */
        void init(misa_module_declaration_base &module) override {
            misa_submodule_base::init(module);
            get_filesystem() = module.filesystem.create_subsystem(name);
        }

    private:
        module_definition_type m_module_definition;
    };
}
