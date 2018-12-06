#include <misaxx/misa_serializeable.h>
#include "misa_serializeable.h"


std::vector<misaxx::misa_serialization_id>
misaxx::misa_serializeable::create_serialization_id_hierarchy(const misaxx::misa_serializeable &self,
                                                              const std::vector<std::vector<misaxx::misa_serialization_id>> &bases) {
    std::vector<misa_serialization_id> result;
    result.push_back(self.get_serialization_id());
    for(const auto &v : bases) {
        std::copy(v.begin(), v.end(), std::back_inserter(result));
    }
    return result;
}
