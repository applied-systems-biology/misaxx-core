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
         * Imports data from filesystem
         * @tparam Data
         * @param t_data
         * @param t_path
         */
        template<class Data> void import_from_filesystem(data<Data> &t_data, const boost::filesystem::path &t_path) {
            if(!t_data) {
                t_data = std::make_shared<Data>();
            }
            t_data->import_from_filesystem(filesystem, t_path);
        }

        /**
         * Exports data to filesystem with a custom path
         * @tparam Data
         * @param t_data
         * @param t_path
         */
        template<class Data> void export_to_filesystem(data<Data> &t_data, const boost::filesystem::path &t_path) {
            if(!t_data) {
                t_data = std::make_shared<Data>();
            }
            t_data->export_to_filesystem(filesystem, t_path);
        }

        /**
         * Exports data to filesystem using a reference data to given path
         * @tparam Data
         * @tparam SourceData
         * @param t_data
         * @param t_source
         */
        template<class Data, class SourceData> void export_to_filesystem(data<Data> &t_data, const data<SourceData> &t_source, const boost::filesystem::path &t_path) {
            if(!t_data) {
                t_data = std::make_shared<Data>();
            }
            if(!t_source) {
                throw std::runtime_error("Source data is not initialized!");
            }
            t_data->export_to_filesystem(filesystem, t_source, t_path);
        }

        /**
         * Initializes a submodule.
         * Please do not forget to setup the definition
         * @tparam Module
         * @param t_module
         * @param t_name
         */
        template<class Module> void init_submodule(submodule<Module> &t_module, const std::string &t_name) {
            t_module.name = t_name;
            t_module.definition().filesystem = filesystem.create_subsystem(t_name);
            t_module.definition().init_data();
        }

    private:

        std::vector<std::shared_ptr<misa_module_data>> m_data;

    };
}
