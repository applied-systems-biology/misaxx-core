//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <unordered_map>
#include <memory>
#include <type_traits>
#include "vfs_entry.h"
#include "vfs_file.h"

namespace misaxx::filesystem {

    struct vfs_folder;

    using folder = std::shared_ptr<vfs_folder>;
    using const_folder = std::shared_ptr<const vfs_folder>;

    /**
     * Used to navigate to non-folders
     * @tparam TargetPtrType
     */
    template<class TargetPtrType> struct as {
        using type = TargetPtrType;
        using element_type = typename type::element_type;
        explicit as(boost::filesystem::path t_segment) : segment(std::move(t_segment)) {

        }
        boost::filesystem::path segment;
    };

    /**
     * A folder in the MISA++ virtual file system
     */
    struct vfs_folder : public vfs_entry {

        using vfs_entry::vfs_entry;
        using iterator = std::unordered_map<std::string, std::shared_ptr<vfs_entry>>::iterator;
        using const_iterator = std::unordered_map<std::string, std::shared_ptr<vfs_entry>>::const_iterator;

        template<class EntryPtr> EntryPtr create(std::string t_name, path t_custom_external = path()) {
            using entry_type = typename EntryPtr::element_type;
            static_assert(std::is_same<std::shared_ptr<entry_type>, EntryPtr>::value, "Wrong entry type!");
            EntryPtr ptr = std::make_shared<entry_type>(std::move(t_name), std::move(t_custom_external));
            ptr->parent = self();
            children.insert({ ptr->name,  ptr});
            return ptr;
        }

        template<class Entry> std::shared_ptr<Entry> insert(std::shared_ptr<Entry> ptr) {
            ptr->parent = self();
            children.insert({ ptr->name,  ptr});
            return ptr;
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

        /**
         * Accesses (which includes creating an entry if necessary)
         * @tparam As
         * @param t_segment
         * @return
         */
        template<class TargetPtrType> TargetPtrType access(boost::filesystem::path t_segment) {
            t_segment.remove_trailing_separator();
            if(t_segment.empty())
                return std::dynamic_pointer_cast<typename TargetPtrType::element_type>(self());

            vfs_folder *current = this;

            // Navigate to subfolders if needed
            for(const auto &seg : t_segment.parent_path()) {
                auto it = current->find(seg.string());
                if(it == end()) {
                    current = create<folder>(seg.string()).get();
                }
                else {
                    current = std::dynamic_pointer_cast<vfs_folder>(it->second).get();
                }
            }

            // Create / access the target element
            auto it = current->find(t_segment.filename().string());
            if(it == end()) {
                return create<TargetPtrType>(t_segment.filename().string());
            }
            else {
                return std::dynamic_pointer_cast<typename TargetPtrType::element_type>(it->second);
            }
        }

        /**
         * Accesses (which includes creating an entry if necessary)
         * @tparam As
         * @param t_segment
         * @return
         */
        template<class TargetPtrType> TargetPtrType at(boost::filesystem::path t_segment) const {
            t_segment.remove_trailing_separator();
            if(t_segment.empty())
                return std::dynamic_pointer_cast<typename TargetPtrType::element_type>(self());

            const vfs_folder *current = this;
            auto current_segment_it = t_segment.begin();

            // Navigate to subfolders if needed
            for(const auto &seg : t_segment.parent_path()) {
                auto it = current->find(seg.string());
                if(it == end()) {
                    throw std::runtime_error("Cannot access path " + (internal_path() / t_segment).string());
                }
                else {
                    current = std::dynamic_pointer_cast<vfs_folder>(it->second).get();
                }
            }

            // Create / access the target element
            auto it = current->find(t_segment.filename().string());
            if(it == end()) {
                throw std::runtime_error("Cannot access path " + (internal_path() / t_segment).string());
            }
            else {
                return std::dynamic_pointer_cast<typename TargetPtrType::element_type>(it->second);
            }
        }

        bool is_file() const override {
            return false;
        }

        bool is_folder() const override {
            return true;
        }

    private:

        std::unordered_map<std::string, std::shared_ptr<vfs_entry>> children;
    };
}

template<class As> typename As::type operator /(misaxx::filesystem::folder &parent, const As &t_segment) {
    return parent->access<typename As::type>(t_segment.segment);
}

misaxx::filesystem::folder operator /(misaxx::filesystem::folder &parent, const std::string &subfolder) {
    return parent / misaxx::filesystem::as<misaxx::filesystem::folder>(subfolder);
}