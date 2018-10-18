//
// Created by rgerst on 17.08.18.
//


#pragma once

#include "misa_file.h"
#include "misa_file_stack.h"

namespace misaxx {
    /**
     * Stack of files without any special annotation
     */
    using misa_generic_file_stack = misa_file_stack<misa_file>;


}