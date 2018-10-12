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

        using files_t = std::unordered_map<std::string, std::shared_ptr<File>>;
        using iterator = typename files_t::iterator;
        using const_iterator = typename files_t::const_iterator;

        files_t files;

        using misa_module_data::misa_module_data;

        /**
         * Returns true if the file stack can import the file.
         * @param t_path
         * @return
         */
        virtual bool supports_file(const boost::filesystem::path &t_path) const {
            return true;
        }

        void import_from_filesystem(const misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            if(!t_module.m_runtime->is_building_schema()) {
                auto t_folder = t_module.filesystem.imported->at<filesystem::const_folder>(t_path);
                for(const auto &kv : *t_folder) {
                    filesystem::file file = std::dynamic_pointer_cast<filesystem::vfs_file>(kv.second);
                    if(file && file->has_external_path() && supports_file(file->external_path())) {
                        File f;
                        f.name = kv.first;
                        f.path = file->external_path();
                        files.insert({ file->name, std::make_shared<File>(std::move(f)) });
                    }
                }
            }
            else {
                // Ensure that the folder exists
                t_module.filesystem.imported->access<filesystem::folder>(t_path);
            }
        }

        template<class Source> void process(misa_module_declaration_base &t_module, const Source &t_source, const boost::filesystem::path &t_path) {
            auto t_folder = t_module.filesystem.exported->access<filesystem::folder>(t_path);

            if(!t_module.m_runtime->is_building_schema()) {
                t_folder->ensure_external_path_exists();

                for(const auto &kv : t_source->files) {
                    if(!supports_file(kv.second->path))
                        continue;
                    filesystem::file file = t_folder->access<filesystem::file>(kv.first);
                    File f;
                    f.name = kv.first;
                    f.path = file->external_path();
                    files.insert({ kv.first, std::make_shared<File>(std::move(f)) });
                }
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

        std::shared_ptr<File> &at(const std::string &t_key) {
            return files.at(t_key);
        }

        const std::shared_ptr<File> &at(const std::string &t_key) const {
            return files.at(t_key);
        }

        auto find(const std::string &t_key) {
            return files.find(t_key);
        }

        auto find(const std::string &t_key) const {
            return files.find(t_key);
        }

        size_t size() const {
            return files.size();
        }

        std::string dataString() override {
            return "generic_file_stack";
        }

    };
}
