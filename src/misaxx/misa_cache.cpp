#include <misaxx/misa_cache.h>
#include <misaxx/runtime/misa_runtime.h>
#include "misaxx/misa_cache.h"


boost::filesystem::path misaxx::misa_cache::get_location_in_filesystem() const {
    const auto fs = misa_runtime_base::instance().get_filesystem();
    const boost::filesystem::path filesystem_import_path = fs.imported->child_external_path(get_location());
    const boost::filesystem::path filesystem_export_path = fs.exported->child_external_path(get_location());

    if(!filesystem_import_path.empty()) {
        return boost::filesystem::path("imported") / filesystem_import_path;
    }
    else if(!filesystem_export_path.empty()) {
        return boost::filesystem::path("exported") / filesystem_export_path;
    }
    else {
        throw std::runtime_error("Unable to locate the cache within the filesystem!");
    }
}

boost::filesystem::path misaxx::misa_cache::get_unique_location_in_filesystem() const {
    const auto fs = misa_runtime_base::instance().get_filesystem();
    const boost::filesystem::path filesystem_import_path = fs.imported->child_external_path(get_unique_location());
    const boost::filesystem::path filesystem_export_path = fs.exported->child_external_path(get_unique_location());

    if(!filesystem_import_path.empty()) {
        return boost::filesystem::path("imported") / filesystem_import_path;
    }
    else if(!filesystem_export_path.empty()) {
        return boost::filesystem::path("exported") / filesystem_export_path;
    }
    else {
        throw std::runtime_error("Unable to locate the cache within the filesystem!");
    }
}
