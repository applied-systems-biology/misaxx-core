//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <unordered_map>
#include <memory>
#include "vfs_entry.h"
#include "vfs_file.h"

namespace misaxx::filesystem {

    struct vfs_folder;

    using folder = std::shared_ptr<vfs_folder>;

    /**
     * Used to navigate to non-folders
     * @tparam TargetPtrType
     */
    template<class TargetPtrType> struct as {
        using type = typename TargetPtrType::element_type;
        explicit as(std::string t_segment) : segment(std::move(t_segment)) {

        }
        std::string segment;
    };

    /**
     * A folder in the MISA++ virtual file system
     */
    struct vfs_folder : public vfs_entry {

        using vfs_entry::vfs_entry;
        using iterator = std::unordered_map<std::string, std::shared_ptr<vfs_entry>>::iterator;
        using const_iterator = std::unordered_map<std::string, std::shared_ptr<vfs_entry>>::const_iterator;

        template<class Entry> std::shared_ptr<Entry> create(std::string t_name, path t_custom_external = path()) {
            std::shared_ptr<Entry> ptr = std::make_shared<Entry>(std::move(t_name), std::move(t_custom_external));
            ptr->parent = self();
            children.insert({ ptr->name,  ptr});
            return ptr;
        }

        template<class Entry> std::shared_ptr<Entry> insert(std::shared_ptr<Entry> ptr) {
            ptr->parent = self();
            children.insert({ ptr->name,  ptr});
            return ptr;
        }

        template<class As> typename As::type& operator /(const As &t_segment) {
            auto it = children.find(t_segment.segment);
            if(it == children.end()) {
                return *create<typename As::type>(t_segment.segment);
            }
            else {
                return *dynamic_cast<typename As::type*>(it->second.get());
            }
        }

        template<class As> const typename As::type& operator /(const As &t_segment) const {
            return *dynamic_cast<typename As::type*>(children.at(t_segment.segment).get());
        }

        vfs_folder& operator /(const std::string &t_segment) {
            return *this / as<folder>(t_segment);
        }

        const vfs_folder& operator /(const std::string &t_segment) const {
            return *this / as<folder>(t_segment);
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

        iterator find(const std::string &t_name) {
            return children.find(t_name);
        }

        const_iterator find(const std::string &t_name) const {
            return children.find(t_name);
        }

        operator folder() {
            return std::dynamic_pointer_cast<vfs_folder>(self().lock());
        }

        /**
         * Ensures that the external folder path exists if it is set.
         * Throws an exception if there is no external path.
         */
        void ensure_external_path_exists() const {
            if(!has_external_path())
                throw std::runtime_error("This VFS folder has no external path!");
            if(!boost::filesystem::exists(external_path()))
                boost::filesystem::create_directories(external_path());
        }

    private:

        std::unordered_map<std::string, std::shared_ptr<vfs_entry>> children;
    };
}