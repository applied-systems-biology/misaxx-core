//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <unordered_map>
#include <memory>
#include "vfs_entry.h"
#include "vfs_file.h"

namespace misaxx::filesystem {

    /**
     * A folder in the MISA++ virtual file system
     */
    struct vfs_folder : public vfs_entry {

        using vfs_entry::vfs_entry;
        using iterator = std::unordered_map<std::string, std::shared_ptr<vfs_entry>>::iterator;
        using const_iterator = std::unordered_map<std::string, std::shared_ptr<vfs_entry>>::const_iterator;

        template<class Entry> Entry& create(std::string t_name, path t_custom_external = path()) {
            std::shared_ptr<Entry> ptr = std::make_shared<Entry>(std::move(t_name), std::move(t_custom_external));
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

        vfs_entry &operator [](const path &t_path) override {
            vfs_entry *ptr = this;
            for(const path &seg : t_path) {
                ptr = &(*ptr / seg.string());
            }
            return *ptr;
        }

        const vfs_entry &operator [](const path &t_path) const override {
            const vfs_entry *ptr = this;
            for(const path &seg : t_path) {
                ptr = &(*ptr / seg.string());
            }
            return *ptr;
        }

        vfs_entry& operator /(const std::string &t_segment) override {
            return *children.at(t_segment);
        }

        const vfs_entry& operator /(const std::string &t_segment) const override {
            return *children.at(t_segment);
        }

        bool empty() const override {
            return children.empty();
        }

        iterator begin() {
            return children.begin();
        }

        const_iterator begin() const {
            return children.begin();
        }

        iterator end() {
            return children.end();
        }

        const_iterator end() const {
            return children.end();
        }

    private:

        std::unordered_map<std::string, std::shared_ptr<vfs_entry>> children;
    };
}