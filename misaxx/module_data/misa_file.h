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

        void import_from_filesystem(const misa_filesystem &t_filesystem, const boost::filesystem::path &t_path) {
            path = t_filesystem.imported->at<filesystem::const_file >(t_path)->external_path();
        }

        void export_to_filesystem(misa_filesystem &t_filesystem, const boost::filesystem::path &t_path) {
            path = t_filesystem.exported->access<filesystem::file >(t_path)->external_path();
        }
    };
}
