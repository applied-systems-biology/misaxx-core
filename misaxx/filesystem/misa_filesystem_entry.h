//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <string>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <misaxx/misa_description_storage.h>

namespace misaxx {

    struct misa_filesystem_entry;

    namespace filesystem {
        using entry = std::shared_ptr<misa_filesystem_entry>;
        using const_entry = std::shared_ptr<const misa_filesystem_entry>;
    }

    /**
     * Available modes of entry types
     */
    enum class misa_filesystem_entry_type {
        unknown,
        imported,
        exported
    };

    /**
     * An entry in the MISA++ virtual filesystem
     */
    struct misa_filesystem_entry : std::enable_shared_from_this<misa_filesystem_entry>, public misa_serializeable {

        using path = boost::filesystem::path;
        using iterator = std::unordered_map<std::string, std::shared_ptr<misa_filesystem_entry>>::iterator;
        using const_iterator = std::unordered_map<std::string, std::shared_ptr<misa_filesystem_entry>>::const_iterator;

        /**
         * Parent node of the current VFS entry
         */
        std::weak_ptr<misa_filesystem_entry> parent;

        /**
       * Internal name of the filesystem entry
       */
        std::string name;

        /**
         * Type of this entry
         */
        misa_filesystem_entry_type type;


        /**
         * Custom external path of this entry.
         * If not set, the path will be loaded from the parent.
         */
        path custom_external;

        /**
         * Contains important information how this filesystem entry is interpreted from a data point
         */
        std::shared_ptr<misa_description_storage> metadata = std::make_shared<misa_description_storage>();

        explicit misa_filesystem_entry(std::string t_name, misa_filesystem_entry_type t_type, path t_custom_external = path()) :
                name(std::move(t_name)), type(t_type), custom_external(std::move(t_custom_external)) {
        }

        misa_filesystem_entry(const misa_filesystem_entry &value) = delete;

        misa_filesystem_entry(misa_filesystem_entry &&value) = default;

        /**
         * Returns the internal path inside this filesystem
         * @return
         */
        path internal_path() const {
            if (parent.expired()) {
                return name;
            } else {
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
            if (!custom_external.empty())
                return custom_external;
            else if (!parent.expired())
                return parent.lock()->external_path() / name;
            else
                throw std::runtime_error("Path " + internal_path().string() + " has no external path!");
        }

        /**
         * Returns a managed pointer to this entry
         * @return
         */
        filesystem::entry self() {
            return shared_from_this();
        }

        filesystem::const_entry self() const {
            return shared_from_this();
        }

        filesystem::entry create(std::string t_name, path t_custom_external = path()) {
            return insert(std::make_shared<misa_filesystem_entry>(std::move(t_name), type, std::move(t_custom_external)));
        }

        filesystem::entry insert(filesystem::entry ptr) {
            ptr->parent = self();
            children.insert({ptr->name, ptr});
            return ptr;
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

        bool empty() const {
            return children.empty();
        }

        /**
         * Ensures that the external folder path exists if it is set.
         * Throws an exception if there is no external path.
         */
        void ensure_external_path_exists() const {
            if (!has_external_path())
                throw std::runtime_error("This VFS folder has no external path!");
            if (!boost::filesystem::exists(external_path()))
                boost::filesystem::create_directories(external_path());
        }

        /**
         * Accesses (which includes creating an entry if necessary)
         * @tparam As
         * @param t_segment
         * @return
         */
        filesystem::entry access(boost::filesystem::path t_segment) {
            t_segment.remove_trailing_separator();
            if (t_segment.empty())
                return self();

            misa_filesystem_entry *current = this;

            // Navigate to subfolders if needed
            for (const auto &seg : t_segment.parent_path()) {
                auto it = current->find(seg.string());
                if (it == end()) {
                    current = create(seg.string()).get();
                } else {
                    current = it->second.get();
                }
            }

            // Create / access the target element
            auto it = current->find(t_segment.filename().string());
            if (it == end()) {
                return create(t_segment.filename().string());
            } else {
                return it->second;
            }
        }

        /**
         * Accesses (which includes creating an entry if necessary)
         * @tparam As
         * @param t_segment
         * @return
         */
        filesystem::const_entry at(boost::filesystem::path t_segment) const {
            t_segment.remove_trailing_separator();
            if (t_segment.empty())
                return self();

            const misa_filesystem_entry *current = this;
            auto current_segment_it = t_segment.begin();

            // Navigate to subfolders if needed
            for (const auto &seg : t_segment.parent_path()) {
                auto it = current->find(seg.string());
                if (it == end()) {
                    throw std::runtime_error("Cannot access path " + (internal_path() / t_segment).string());
                } else {
                    current = it->second.get();
                }
            }

            // Create / access the target element
            auto it = current->find(t_segment.filename().string());
            if (it == end()) {
                throw std::runtime_error("Cannot access path " + (internal_path() / t_segment).string());
            } else {
                return it->second;
            }
        }

        /**
         * Returns true if this filesystem has given subpath
         * @param t_segment
         * @return
         */
        bool has_subpath(boost::filesystem::path t_segment) const {
            t_segment.remove_trailing_separator();
            if (t_segment.empty())
                return true;

            const misa_filesystem_entry *current = this;
            auto current_segment_it = t_segment.begin();

            // Navigate to subfolders if needed
            for (const auto &seg : t_segment.parent_path()) {
                auto it = current->find(seg.string());
                if (it == end()) {
                    return false;
                } else {
                    current = it->second.get();
                }
            }

            // Create / access the target element
            auto it = current->find(t_segment.filename().string());
            return !(it == end());
        }

        void from_json(const nlohmann::json &t_json) override {
            throw std::runtime_error("Not implemented");
        }

        void to_json(nlohmann::json &t_json) const override {
            throw std::runtime_error("Not implemented");
        }

        void to_json_schema(const misa_json_schema &t_schema) const override {
            t_schema.declare_optional<std::string>("external-path");
            for(const auto &kv : children) {
                kv.second->to_json_schema(t_schema.resolve("children", kv.first));
            }
            metadata->to_json_schema(t_schema.resolve("metadata"));
        }

        misa_serialization_id get_serialization_id() const override {
            return misa_serialization_id("misa", "filesystem/entry");
        }

    private:

        std::unordered_map<std::string, std::shared_ptr<misa_filesystem_entry>> children;
    };
}
