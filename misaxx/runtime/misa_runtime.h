//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <pattxx/default_runtime.h>
#include <misaxx/filesystem/misa_filesystem.h>

namespace misaxx {

    /**
     * Root instantiator for misaxx::misa_runtime
     * @tparam RootModule
     */
    template <class RootModule> class misa_runtime_root_instantiator {
    protected:
        using node_type = pattxx::nodes::dispatcher_node<RootModule, pattxx::dispatcher>;
        using module_declaration_type = typename RootModule::module_declaration_type;

        explicit misa_runtime_root_instantiator(std::string t_root_name) : m_root_name(std::move(t_root_name)) {
        }

        /**
         * Instantiates the root
         * @param rt
         * @return
         */
        std::unique_ptr<node_type> instantiate_root(pattxx::runtime &rt) {
            module_declaration_type definition;
            definition.m_runtime = &rt;
            definition.filesystem = m_filesystem;
            return std::make_unique<node_type >(m_root_name, &rt, std::move(definition));
        }

        std::string m_root_name;
        misa_filesystem m_filesystem;
    };

    template<class RootModule> class misa_runtime : public pattxx::default_runtime<RootModule, misa_runtime_root_instantiator<RootModule>> {
    public:

        using node_type = typename pattxx::default_runtime<RootModule>::node_type;

        using pattxx::default_runtime<RootModule, misa_runtime_root_instantiator<RootModule>>::default_runtime;

        /**
         * Returns the filesystem instance of this runtime
         * @return
         */
        misa_filesystem &get_filesystem() {
            return this->m_filesystem;
        }
    };
}
