#include <misaxx/misa_mutable_module_info.h>

namespace misaxx {
    inline misaxx::misa_module_info module_info() {
        misaxx::misa_mutable_module_info info;
        info.set_name("misaxx");
        info.set_version("0.1.0");
        info.set_description("MISA++ Core");
        return info;
    }
}