#include <misaxx/core/misa_cache.h>
#include "runtime/misa_runtime.h"
#include <misaxx/core/misa_cache.h>

boost::filesystem::path misaxx::misa_cache::get_internal_unique_location() {
    boost::filesystem::path relative = boost::filesystem::relative(get_unique_location(), get_location());
    return get_internal_location() / relative;
}
