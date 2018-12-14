//
// Created by rgerst on 28.11.18.
//

#include "misa_exported_attachments.h"

using namespace misaxx;

void misa_exported_attachments::suggest_export_location(const misa_filesystem &t_filesystem,
                                                                const boost::filesystem::path &t_path) {
    if(t_path.parent_path().empty())
        throw std::runtime_error("The path must be a subfolder!");
    suggest_export_location(t_filesystem, t_path.parent_path(), misa_description_storage::with(misa_file_description(t_path.filename())));
}
