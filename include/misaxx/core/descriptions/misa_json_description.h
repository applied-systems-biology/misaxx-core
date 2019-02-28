//
// Created by rgerst on 26.02.19.
//

#pragma once

#include "misa_file_description.h"

namespace misaxx {
    struct misa_json_description : public misa_file_description {

        using misa_file_description::misa_file_description;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;
    };
}




