//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_module_declaration_base.h"
#include "misa_submodule.h"

namespace misaxx {

    /**
     * The module definition contains which data is available inside the module, which data is exported and
     * which modules are included.
     * The module definition is combined with the module dispatcher (misaxx::misa_module) to build the
     * final module.
     */
    class misa_module_declaration : public misa_module_declaration_base {
    public:
        using metadata = pattxx::metadata;
        template<class Module> using submodule = misa_submodule<Module>;
        template<typename T> using data = std::shared_ptr<T>;
    protected:

        /**
         * Declares data for the module definition
         * @tparam T
         * @tparam Args
         * @param args
         * @return
         */
        template<class T, typename... Args> data<T> declare_data(std::string t_name, metadata t_metadata = metadata(), Args&&... args) {
            static_assert(std::is_base_of<misa_module_data, T>::value, "Only module data is supported!");
            auto ptr = std::shared_ptr<T>(new T(*this, std::move(t_name), std::move(t_metadata), std::forward<Args>(args)...));
            m_data.push_back(ptr);
            return ptr;
        }

        /**
         * Declares a module as submodule
         * @tparam Module
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<class Module> submodule<Module> declare_submodule(std::string t_name, metadata t_metadata = metadata()) {
            return submodule<Module>(*this, std::move(t_name), std::move(t_metadata));
        }

    private:

        std::vector<std::shared_ptr<misa_module_data>> m_data;

    };
}
