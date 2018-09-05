//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "../filesystem/vfs_folder.h"
#include "../misa_module_data.h"
#include "misaxx/misa_module_declaration_base.h"
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

        void clear() override {
            misa_module_data::clear();
            files.clear();
        }

        void operator <<(const filesystem::folder &t_folder) {
            if(has_value)
                return;
            for(const auto &kv : *t_folder) {
                filesystem::file file = std::dynamic_pointer_cast<filesystem::vfs_file>(kv.second);
                if(file && file->has_external_path()) {
                    File f(*parent_module, kv.first);
                    f.init(*parent_module);
                    f << file;
                    files.insert({ file->name, std::move(f) });
                }
            }
            has_value = true;
        }

        void operator <<(const misa_file &t_reference_file) {
            if(has_value)
                return;
            if(!t_reference_file.has_value)
                throw std::runtime_error("Reference file does not contain any value!");
            auto f = parent_module->filesystem.exported->create<filesystem::file>(t_reference_file.path.filename().string());
            files.insert({ f->name, std::move(f) });
            has_value = true;
        }

        template<class SourceFile> void operator <<(const misa_file_stack<SourceFile> &t_reference_stack) {
            if(has_value)
                return;
            if(!t_reference_stack.has_value)
                throw std::runtime_error("Reference stack does not contain any value!");
            filesystem::folder target_folder = parent_module->filesystem.exported / name;
            target_folder->ensure_external_path_exists();

            auto external = target_folder->external_path();

            for(const auto &kv : t_reference_stack.files) {
                filesystem::file file = target_folder->template create<filesystem::file>(kv.second.name);
                File f(*parent_module, kv.first);
                f.init(*parent_module);
                f << file;
                files.insert({ kv.first, std::move(f) });
            }
            has_value = true;
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
