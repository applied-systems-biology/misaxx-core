//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <pattxx/dispatcher.h>
#include "filesystem/vfs_folder.h"

namespace misaxx {

    struct misa_module_definition;

    struct misa_submodule_base {
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

        explicit misa_submodule(misa_module_definition &t_parent, std::string t_name, pattxx::metadata t_metadata) :
                m_name(std::move(t_name)),
                m_metadata(std::move(t_metadata)) {
            if (m_name.empty())
                throw std::runtime_error("The name of the submodule is empty!");
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

        const std::string &get_name() const {
            return m_name;
        }

        const pattxx::metadata &get_metadata() const {
            return m_metadata;
        }

        bool has_instance() const {
            return m_module != nullptr;
        }

    private:
        std::string m_name;
        pattxx::metadata m_metadata;
        module_definition_type m_module_definition;
    };
}
