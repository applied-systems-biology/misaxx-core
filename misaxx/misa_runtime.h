//
// Created by rgerst on 15.08.18.
//


#pragma once

#include <pattxx/default_runtime.h>
#include "filesystem/virtual_filesystem.h"

namespace misaxx {

    /**
     * Root instantiator for misaxx::misa_runtime
     * @tparam RootModule
     */
    template <class RootModule> class misa_runtime_root_instantiator {
    protected:
        using node_type = pattxx::nodes::dispatcher_node<RootModule, pattxx::dispatcher>;

        explicit misa_runtime_root_instantiator(std::string t_root_name) : m_root_name(std::move(t_root_name)) {

        }

        /**
         * Instantiates the root
         * @param rt
         * @return
         */
        std::unique_ptr<node_type> instantiate_root(pattxx::runtime &rt) {
            return std::make_unique<node_type >(m_root_name, &rt, m_filesystem->root);
        }

        std::string m_root_name;
        std::shared_ptr<filesystem::virtual_filesystem> m_filesystem;
    };

    template<class RootModule> class misa_runtime : public pattxx::default_runtime<RootModule, misa_runtime_root_instantiator<RootModule>> {
    public:

        using node_type = typename pattxx::default_runtime<RootModule>::node_type;

        using pattxx::default_runtime<RootModule, misa_runtime_root_instantiator<RootModule>>::default_runtime;

    };
}
