#include <misaxx/misa_cache.h>
#include <misaxx/runtime/misa_runtime.h>
#include <misaxx-helpers/filesystem.h>
#include "misaxx/misa_cache.h"


boost::filesystem::path misaxx::misa_cache::get_location_in_filesystem() const {
    const auto fs = misa_runtime_base::instance().get_filesystem();
    const auto found_entry = fs.find_external_path(get_location());

    if(!static_cast<bool>(found_entry)) {
        throw std::runtime_error("Unable to locate the cache within the filesystem!");
    }
    else if(found_entry->type == misa_filesystem_entry_type::imported) {
        return boost::filesystem::path("imported") /
        found_entry->internal_path() /
        cxxh::relativize_to_direct_parent(found_entry->external_path(), get_location());
    }
    else if(found_entry->type == misa_filesystem_entry_type::exported) {
        return boost::filesystem::path("exported") /
               found_entry->internal_path() /
               cxxh::relativize_to_direct_parent(found_entry->external_path(), get_location());
    }
    else {
        throw std::runtime_error("Unsupported filesystem entry type!");
    }
}

boost::filesystem::path misaxx::misa_cache::get_unique_location_in_filesystem() const {
    const auto fs = misa_runtime_base::instance().get_filesystem();
    const auto found_entry = fs.find_external_path(get_unique_location());

    if(!static_cast<bool>(found_entry)) {
        throw std::runtime_error("Unable to locate the cache within the filesystem!");
    }
    else if(found_entry->type == misa_filesystem_entry_type::imported) {
        return found_entry->internal_path() /
               cxxh::relativize_to_direct_parent(found_entry->external_path(), get_unique_location());
    }
    else if(found_entry->type == misa_filesystem_entry_type::exported) {
        return found_entry->internal_path() /
               cxxh::relativize_to_direct_parent(found_entry->external_path(), get_unique_location());
    }
    else {
        throw std::runtime_error("Unsupported filesystem entry type!");
    }
}
