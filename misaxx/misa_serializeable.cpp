#include <misaxx/misa_serializeable.h>
#include "misa_serializeable.h"

std::vector<misaxx::misa_serialization_id>
misaxx::misa_serializeable::create_serialization_id_hierarchy(misaxx::misa_serialization_id self,
                                                              const std::vector<std::vector<misaxx::misa_serialization_id>> &bases) {
    std::vector<misa_serialization_id> result;
    for(const auto &v : bases) {
        std::copy(v.begin(), v.end(), std::back_inserter(result));
    }
    result.emplace_back(std::move(self));
    return result;
}
