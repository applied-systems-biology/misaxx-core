#include <misaxx/core/misa_cache.h>

boost::filesystem::path misaxx::misa_cache::get_internal_unique_location() const {
    boost::filesystem::path relative = boost::filesystem::relative(get_unique_location(), get_location());
    return get_internal_location() / relative;
}
