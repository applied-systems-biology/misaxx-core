//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <string>
#include <stdexcept>
#include <boost/filesystem.hpp>

namespace misaxx::filesystem {

    /**
     * An entry in the MISA++ virtual filesystem
     */
    struct vfs_entry {

        using path = boost::filesystem::path;

        /**
         * Parent node of the current misa_entry
         */
        vfs_entry *parent = nullptr;

        /**
         * Internal name of the filesystem entry
         */
        std::string name;

        /**
         * Custom external path of this entry.
         * If not set, the path will be loaded from the parent.
         */
        path custom_external;

        explicit vfs_entry(std::string t_name = "", path t_custom_external = path()) : name(std::move(t_name)), custom_external(std::move(t_custom_external)){
        }

        vfs_entry(const vfs_entry &value) = delete;

        vfs_entry(vfs_entry &&value) = default;

        /**
         * Returns the internal path inside this filesystem
         * @return
         */
        path internal_path() const {
            if(parent == nullptr) {
                return name;
            }
            else {
                return parent->internal_path() / name;
            }
        }

        /**
         * Returns true if this entry is associated to an external path
         * @return
         */
        bool has_external_path() const {
            return !external_path().empty();
        }

        /**
         * Returns the external path of this node.
         * @return
         */
        path external_path() const {
            if(!custom_external.empty())
                return custom_external;
            else if(parent != nullptr)
                return parent->external_path() / name;
            else
                throw std::runtime_error("Path " + internal_path().string() + " has no external path!");
        }

        virtual const vfs_entry &operator [](const path &t_path) const {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual vfs_entry &operator [](const path &t_path) {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual const vfs_entry& operator /(const std::string &t_segment) const {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual vfs_entry& operator /(const std::string &t_segment) {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual bool empty() const = 0;
    };
}
