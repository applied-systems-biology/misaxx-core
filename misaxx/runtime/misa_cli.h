//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <misaxx/runtime/misa_runtime.h>
#include <misaxx/misa_multiobject_root.h>
#include <misaxx/misa_cached_data_base.h>
#include <misaxx/attachments/misa_location.h>
#include "misa_cli_base.h"

namespace misaxx {

    /**
     * MISA++ CLI
     * @tparam Module The module that should be executed
     * @tparam Runtime
     */
    template<class Module> class misa_cli : public misa_cli_base {

    public:

        using module_type = Module;
        using runtime_type = misa_runtime<Module>;

        explicit misa_cli(std::string t_root_name) :
            misa_cli_base(runtime_type::create_instance(std::move(t_root_name))) {

        }

        /**
         * Returns the internal runtime instance
         * @return
         */
        std::shared_ptr<runtime_type> get_module_runtime() const {
            return std::dynamic_pointer_cast<runtime_type>(get_runtime());
        }

    private:

        boost::filesystem::path m_parameter_schema_path;

    };
}