//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <misaxx/core/runtime/misa_runtime.h>
#include <misaxx/core/misa_multiobject_root.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/misa_module_info.h>
#include <misaxx/core/runtime/misa_cli_base.h>

namespace misaxx {

    /**
     * MISA++ CLI
     * @tparam Module The module that should be executed
     * @tparam Runtime
     */
    template<class Module> class misa_cli : public misa_cli_base {

    public:

        using module_type = Module;
        using root_module_type = misa_multiobject_root<module_type>;
        using runtime_type = misa_runtime<root_module_type>;

        explicit misa_cli(misa_module_info t_module_info) :
            misa_cli_base(runtime_type::create_instance(std::move(t_module_info))) {

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