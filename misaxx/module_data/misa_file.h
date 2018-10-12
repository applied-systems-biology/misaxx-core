//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "misaxx/misa_module_declaration_base.h"
#include "../misa_module_data.h"
#include "../filesystem/vfs_file.h"

namespace misaxx {
    struct misa_file : public misa_module_data {

        std::string name;
        boost::filesystem::path path;

        using misa_module_data::misa_module_data;

        void import_from_filesystem(const misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            if(!t_module.m_runtime->is_building_schema()) {
                const auto &vfs = t_module.filesystem.imported->at<filesystem::const_file >(t_path);
                vfs->data_string = dataString();
                path = vfs->external_path();
            }
            else {
                // Ensure that the file exists
                t_module.filesystem.imported->access<filesystem::file>(t_path)->data_string = dataString();
            }
        }

        void export_to_filesystem(misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            if(!t_module.m_runtime->is_building_schema()) {
                const auto &vfs = t_module.filesystem.exported->access<filesystem::file>(t_path);
                vfs->data_string = dataString();
                path = vfs->external_path();
            }
            else {
                // Ensure that the file exists
                t_module.filesystem.exported->access<filesystem::file>(t_path)->data_string = dataString();
            }
        }

        std::string dataString() override {
            return "file";
        }
    };
}
