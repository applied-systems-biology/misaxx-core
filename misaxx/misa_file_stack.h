//
// Created by rgerst on 10.08.18.
//


#pragma once

#include "misaxx/filesystem/vfs_folder.h"

namespace misaxx {

    /**
     * Stack of files with any type.
     */
    struct misa_file_stack {

        using path = boost::filesystem::path;
        std::unordered_map<std::string, path> files;

        /**
         * Creates an imported file stack based on the files located in the virtual filesystem folder
         * @param t_folder
         * @return
         */
        static void vfs_import(misa_file_stack &result, const filesystem::vfs_entry &t_folder_path) {
            const auto &t_folder = dynamic_cast<const filesystem::vfs_folder&>(t_folder_path);
            for(const auto &kv : t_folder) {
                const auto *ptr = dynamic_cast<const filesystem::vfs_file*>(kv.second.get());
                if(ptr != nullptr && ptr->has_external_path()) {
                    result.files.insert({ ptr->name, ptr->external_path() });
                }
            }
        }

        /**
         * Creates an exported file stack based on the files in a reference stack.
         * @param t_reference_stack
         * @param t_root_folder
         * @param t_name
         * @return
         */
        static void vfs_export(misa_file_stack &result, const misa_file_stack &t_reference_stack, filesystem::vfs_entry &t_root_folder_path, std::string t_name) {

            auto &t_root_folder = dynamic_cast<filesystem::vfs_folder&>(t_root_folder_path);

            if(!t_root_folder.has_external_path()) {
                throw std::runtime_error("Cannot export file stack into " + t_root_folder.internal_path().string() + ", as it has not external path!");
            }

            path external = t_root_folder.external_path();
            auto &target_folder = t_root_folder.create<filesystem::vfs_folder>(t_name);

            for(const auto &kv : t_reference_stack.files) {
                result.files.insert({ kv.first, external / t_name / kv.second.filename() });
                target_folder.create<filesystem::vfs_file>(kv.second.filename().string());
            }
        }

    };
}
