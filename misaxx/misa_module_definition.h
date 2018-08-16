//
// Created by rgerst on 10.08.18.
//


#pragma once
#include <memory>
#include <pattxx/metadata.h>
#include "misa_submodule.h"

namespace misaxx {

    struct misa_module_data;

    /**
     * The module definition contains which data is available inside the module, which data is exported and
     * which modules are included.
     * The module definition is combined with the module dispatcher (misaxx::misa_module) to build the
     * final module.
     */
    struct misa_module_definition {

        using metadata = pattxx::metadata;
        template<class Module> using submodule = misa_submodule<Module>;

    protected:

        /**
         * Creates data for the module definition
         * @tparam T
         * @tparam Args
         * @param args
         * @return
         */
        template<class T, typename... Args> T data(std::string t_name, metadata t_metadata, Args&&... args) {
            static_assert(std::is_base_of<misa_module_data, T>::value, "Only module data is supported!");
            return T(*this, std::move(t_name), std::move(t_metadata), std::forward<Args>(args)...);
        }

        /**
         * Imports a module as submodule
         * @tparam Module
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<class Module> submodule<Module> imported(std::string t_name, metadata t_metadata) {
            return submodule<Module>(*this, std::move(t_name), std::move(t_metadata));
        }

    public:

        const filesystem::folder& get_filesystem() const {
            return m_filesystem;
        }

        filesystem::folder& get_filesystem() {
            return m_filesystem;
        }

        filesystem::folder vfs_import() {
            return *m_filesystem / "import";
        }

        filesystem::folder vfs_export() {
            return *m_filesystem / "export";
        }

        filesystem::folder vfs_modules() {
            return *m_filesystem / "modules";
        }

    private:

        filesystem::folder m_filesystem;
    };
}
