//
// Created by rgerst on 26.02.19.
//

#pragma once

#include "misa_file_pattern.h"

namespace misaxx {
    struct misa_json_pattern : public misa_file_pattern {

        misa_json_pattern();

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;
    };
}




