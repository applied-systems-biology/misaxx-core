//
// Created by rgerst on 26.02.19.
//

#include <misaxx/core/accessors/misa_json.h>

#include "misaxx/core/accessors/misa_json.h"

bool misaxx::misa_json::exists() const {
    auto lock = data->shared_lock();
    lock.lock();
    return data->can_pull();
}
