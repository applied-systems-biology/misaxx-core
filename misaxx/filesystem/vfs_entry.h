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
         * Parent node of the current VFS entry
         */
        std::weak_ptr<vfs_entry> parent;

        /**
         * Weak_ptr of this entry. Used to create shared_ptr of this node
         */
        std::weak_ptr<vfs_entry> self;

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
            if(parent.expired()) {
                return name;
            }
            else {
                return parent.lock()->internal_path() / name;
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
            else if(!parent.expired())
                return parent.lock()->external_path() / name;
            else
                throw std::runtime_error("Path " + internal_path().string() + " has no external path!");
        }

        virtual bool empty() const = 0;
    };

    using entry = std::shared_ptr<vfs_entry>;
}
