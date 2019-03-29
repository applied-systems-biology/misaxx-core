//
// Created by rgerst on 29.03.19.
//

#pragma once
#include <misaxx/core/misa_dispatcher.h>

namespace misaxx {
    /**
     * Base class for a MISA++ module
     */
    struct misa_module_base : public misa_dispatcher {
        using misa_dispatcher::misa_dispatcher;
    };
}