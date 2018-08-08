//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <string>
#include <stdexcept>

namespace misaxx::filesystem {
    struct misa_entry {

        std::string name;

        explicit misa_entry(std::string t_name = "") : name(std::move(t_name)) {
        }

        misa_entry(const misa_entry &value) = delete;

        misa_entry(misa_entry &&value) = default;

        virtual const misa_entry& operator /(const std::string &t_segment) const {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual misa_entry& operator /(const std::string &t_segment) {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual bool empty() const = 0;
    };
}
