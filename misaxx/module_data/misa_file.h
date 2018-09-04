//
// Created by rgerst on 17.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "../misa_module_definition_base.h"
#include "../misa_module_data.h"
#include "../filesystem/vfs_file.h"

namespace misaxx {
    struct misa_file : public misa_module_data {
        boost::filesystem::path path;

        using misa_module_data::misa_module_data;

        void clear() override {
            misa_module_data::clear();
            path = boost::filesystem::path();
        }

        void operator <<(const filesystem::file &t_file) {
            if(has_value)
                return;
            path = t_file->external_path();
            has_value = true;
        }

        void operator <<(const misa_file &t_reference_file) {
            if(has_value)
                return;
            auto f = m_module->filesystem.exported->create<filesystem::file>(t_reference_file.path.filename().string());
            path = f->external_path();
            has_value = true;
        }
    };
}
