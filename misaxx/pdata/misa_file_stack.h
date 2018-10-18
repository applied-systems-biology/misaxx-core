//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "misa_pdata.h"
#include "misaxx/misa_module_declaration_base.h"
#include "misa_file.h"

namespace misaxx {

    /**
     * Stack of files with any type.
     */
    template<class File> struct misa_file_stack : public misa_pdata {

        using files_t = std::unordered_map<std::string, std::shared_ptr<File>>;
        using iterator = typename files_t::iterator;
        using const_iterator = typename files_t::const_iterator;

        files_t files;

        using misa_pdata::misa_pdata;

        /**
         * Returns true if the file stack can import the file.
         * @param t_path
         * @return
         */
        virtual bool supports_file(const boost::filesystem::path &t_path) const {
            return true;
        }

        void import_from_filesystem(const misa_module_declaration_base &t_module, const boost::filesystem::path &t_path) {
            std::cout << "[Data] Importing " << t_path.string() << " as " << dataString() << std::endl;
            t_module.filesystem.imported->access(t_path)->data_string = dataString();
            if(!t_module.m_runtime->is_building_schema()) {
                const auto &t_folder = t_module.filesystem.imported->access(t_path);
                t_folder->data_string = dataString();

                if(boost::filesystem::exists(t_folder->external_path())) {
                    boost::filesystem::directory_iterator it(t_folder->external_path());
                    while(it != boost::filesystem::directory_iterator()) {
                        boost::filesystem::path external_path = *it++;
                        if(boost::filesystem::is_regular_file(external_path) && supports_file(external_path)) {
                            filesystem::entry e = t_folder->access(external_path.filename());
                            File f;
                            f.name = external_path.filename().string();
                            f.path = external_path;
                            files.insert({ f.name, std::make_shared<File>(std::move(f)) });
                        }
                    }
                }
            }
        }

        template<class Source> void process(misa_module_declaration_base &t_module, const Source &t_source, const boost::filesystem::path &t_path) {
            std::cout << "[Data] Processing " << t_source->dataString() << " into " << t_path.string() << " as " << dataString() << std::endl;
            const auto &t_folder = t_module.filesystem.exported->access(t_path);
            t_folder->data_string = dataString();

            if(!t_module.m_runtime->is_building_schema()) {
                t_folder->ensure_external_path_exists();

                for(const auto &kv : t_source->files) {
                    if(!supports_file(kv.second->path))
                        continue;
                    filesystem::entry file = t_folder->access(kv.first);
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
