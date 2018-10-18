//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/misa_module_declaration_base.h"
#include "misa_pdata.h"
#include "misaxx/filesystem/misa_filesystem_entry.h"

namespace misaxx {
    struct misa_file : public misa_pdata {

        std::string name;
        boost::filesystem::path path;

        using misa_pdata::misa_pdata;

        void import_from_filesystem(const misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            std::cout << "[Data] Importing " << t_path.string() << " as " << dataString() << std::endl;
            t_module.filesystem.imported->access(t_path)->data_string = dataString();

            if(!t_module.m_runtime->is_building_schema()) {
                const auto &vfs = t_module.filesystem.imported->access(t_path);
                vfs->data_string = dataString();
                path = vfs->external_path();
            }
        }

        void export_to_filesystem(misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            std::cout << "[Data] Exporting " << t_path.string() << " as " << dataString() << std::endl;
            t_module.filesystem.exported->access(t_path)->data_string = dataString();
            if(!t_module.m_runtime->is_building_schema()) {
                const auto &vfs = t_module.filesystem.exported->access(t_path);
                vfs->data_string = dataString();
                path = vfs->external_path();
            }
        }

        std::string dataString() override {
            return "file";
        }
    };
}
