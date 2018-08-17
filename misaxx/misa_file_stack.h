//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "misaxx/filesystem/vfs_folder.h"
#include "misa_module_data.h"
#include "misa_module_definition_base.h"
#include "misa_file.h"

namespace misaxx {

    /**
     * Stack of files with any type.
     */
    template<class File> struct misa_file_stack : public misa_module_data {

        using files_t = std::unordered_map<std::string, File>;
        using iterator = typename files_t::iterator;
        using const_iterator = typename files_t::const_iterator;

        files_t files;

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
                filesystem::file file = std::dynamic_pointer_cast<filesystem::vfs_file>(kv.second);
                if(file && file->has_external_path()) {
                    File f(t_module, kv.first);
                    f.init(t_module, file);
                    files.insert({ file->name, std::move(f) });
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
        void init(misa_module_definition_base &t_module, const misa_file_stack<File> &t_reference_stack, bool force = false) {
            if(has_value && !force)
                return;

            auto &target_folder = *t_module.filesystem.exported / name;
            target_folder.ensure_external_path_exists();

            auto external = target_folder.external_path();

            for(const auto &kv : t_reference_stack.files) {
                filesystem::file file = target_folder.template create<filesystem::vfs_file>(kv.second.name);
                File f(t_module, kv.first);
                f.init(t_module, file);
                files.insert({ kv.first, std::move(f) });
            }
        }

        iterator begin() {
            return files.begin();
        }

        const_iterator begin() const {
            return files.begin();
        }

        iterator end() {
            return files.end();
        }

        const_iterator end() const {
            return files.end();
        }

    };
}
