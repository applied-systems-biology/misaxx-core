//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <boost/filesystem.hpp>
#include "../misa_filesystem.h"

namespace misaxx::filesystem::importers {

    /**
     * Imports a filesystem from an input folder.
     */
    struct default_importer {
        boost::filesystem::path input_path;

        void import_into(boost::filesystem::path &subdir, misa_folder &folder) {

            using namespace boost::filesystem;

            misa_stack local_stack(subdir.filename().string());

            directory_iterator it { subdir };
            while(it != directory_iterator()) {
                path file = *it++;
                if(is_regular_file(file)) {
                    local_stack.files.insert({ file.filename().string(), file.string() });
                }
                else if(is_directory(file)) {
                    misa_folder subf(file.filename().string());
                    import_into(file, subf);
                    folder.insert(std::make_unique<misa_folder>(std::move(subf)));
                }
            }

            if(!local_stack.empty()) {
                folder.insert(std::make_unique<misa_stack>(std::move(local_stack)));
            }
        }

        misa_filesystem import() {
            misa_filesystem result;
            import_into(input_path, result);
            return result;
        }
    };
}
