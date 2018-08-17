//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "misaxx/filesystem/vfs_folder.h"
#include "misa_module_data.h"
#include "misa_module_definition_base.h"

namespace misaxx {

    /**
     * Stack of files with any type.
     */
    struct misa_file_stack : public misa_module_data {

        using path = boost::filesystem::path;
        std::unordered_map<std::string, path> files;

        using misa_module_data::misa_module_data;

        /**
         * Initializes this file stack from the virtual filesystem.
         * @param t_module
         * @param t_folder
         * @param force
         */
        void init(misa_module_definition_base &t_module, const filesystem::folder &t_folder, bool force = false) {
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
        * Initializes this file stack from another file stack.
        * The files will be placed into /exported/<name>
        * @param t_module
        * @param t_reference_stack
        * @param force
        */
        void init(misa_module_definition_base &t_module, const misa_file_stack &t_reference_stack, bool force = false) {
            if(has_value && !force)
                return;

            auto &target_folder = *t_module.filesystem.exported / name;
            target_folder.ensure_external_path_exists();

            path external = target_folder.external_path();

            for(const auto &kv : t_reference_stack.files) {
                files.insert({ kv.first, external / kv.second.filename() });
                target_folder.create<filesystem::vfs_file>(kv.second.filename().string());
            }
        }

    };
}
