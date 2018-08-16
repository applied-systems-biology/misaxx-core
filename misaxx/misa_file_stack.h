//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "misaxx/filesystem/vfs_folder.h"
#include "misa_module_data.h"
#include "misa_module_definition.h"

namespace misaxx {

    /**
     * Stack of files with any type.
     */
    struct misa_file_stack : public misa_module_data {

        using path = boost::filesystem::path;
        std::unordered_map<std::string, path> files;

        using misa_module_data::misa_module_data;

        /**
         * Creates an imported file stack based on the files located in the virtual filesystem folder
         * @param t_folder
         * @return
         */
        void imported_data(const filesystem::folder &t_folder, bool force = false) {
            if(has_value && !force)
                return;
            for(const auto &kv : *t_folder) {
                const auto *ptr = dynamic_cast<const filesystem::vfs_file*>(kv.second.get());
                if(ptr != nullptr && ptr->has_external_path()) {
                    files.insert({ ptr->name, ptr->external_path() });
                }
            }
            has_value = true;
        }

        /**
         * Creates an exported file stack based on the files in a reference stack.
         * @param t_reference_stack
         * @param t_root_folder
         * @param t_name
         * @return
         */
        void exported_data(const misa_file_stack &t_reference_stack, bool force = false) {
            if(has_value && !force)
                return;

            auto &target_folder = *module->vfs_export() / name;

            if(!target_folder.has_external_path()) {
                throw std::runtime_error("Cannot export file stack into " + target_folder.internal_path().string() + ", as it has no external path!");
            }

            path external = target_folder.external_path();

            for(const auto &kv : t_reference_stack.files) {
                files.insert({ kv.first, external / kv.second.filename() });
                target_folder.create<filesystem::vfs_file>(kv.second.filename().string());
            }
        }

    };
}
