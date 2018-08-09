//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <unordered_map>
#include <memory>
#include "misa_entry.h"
#include "misa_file.h"

namespace misaxx::filesystem {
    struct misa_folder : public misa_entry {

        using misa_entry::misa_entry;

        template<class Entry> Entry& create(std::string t_name) {
            std::shared_ptr<Entry> ptr = std::make_shared<Entry>(std::move(t_name));
            auto &ref = *ptr;
            ref.parent = this;
            children.insert({ ptr->name,  std::move(ptr)});
            return ref;
        }

        template<class Entry> Entry& insert(std::shared_ptr<Entry> ptr) {
            auto &ref = *ptr;
            ref.parent = this;
            children.insert({ ptr->name,  std::move(ptr)});
            return ref;
        }

        misa_entry& operator /(const std::string &t_segment) override {
            return *children.at(t_segment);
        }

        const misa_entry& operator /(const std::string &t_segment) const override {
            return *children.at(t_segment);
        }

        bool empty() const override {
            return children.empty();
        }

    private:

        std::unordered_map<std::string, std::shared_ptr<misa_entry>> children;
    };
}