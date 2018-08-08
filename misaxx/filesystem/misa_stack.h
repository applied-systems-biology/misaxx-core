//
// Created by rgerst on 08.08.18.
//


#pragma once

#include "misa_entry.h"
#include <string>
#include <unordered_map>

namespace misaxx::filesystem {
    struct misa_stack : public misa_entry {
        using paths_t = std::unordered_map<std::string, std::string>;
        paths_t files;

        using misa_entry::misa_entry;

        bool empty() const override {
            return files.empty();
        }
    };
}