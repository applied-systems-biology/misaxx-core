//
// Created by rgerst on 08.08.18.
//


#pragma once

#include <string>
#include <stdexcept>
#include <boost/filesystem.hpp>

namespace misaxx::filesystem {

    struct misa_entry {

        /**
         * Parent node of the current misa_entry
         */
        misa_entry *parent = nullptr;

        /**
         * Internal name of the filesystem entry
         */
        std::string name;

        /**
         * Custom external path of this entry.
         * If not set, the path will be loaded from the parent.
         */
        boost::filesystem::path custom_external;

        explicit misa_entry(std::string t_name = "") : name(std::move(t_name)){
        }

        misa_entry(const misa_entry &value) = delete;

        misa_entry(misa_entry &&value) = delete;

        /**
         * Returns the internal path inside this filesystem
         * @return
         */
        boost::filesystem::path internal_path() const {
            if(parent == nullptr) {
                return name;
            }
            else {
                return parent->internal_path() / name;
            }
        }

        /**
         * Returns the external path of this node.
         * @return
         */
        boost::filesystem::path external_path() const {
            if(!custom_external.empty())
                return custom_external;
            else if(parent != nullptr)
                return parent->external_path() / name;
            else
                throw std::runtime_error("Path " + internal_path().string() + " has no external path!");
        }

        virtual const misa_entry& operator /(const std::string &t_segment) const {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual misa_entry& operator /(const std::string &t_segment) {
            throw std::runtime_error("This filesystem node cannot be navigated!");
        }

        virtual bool empty() const = 0;
    };
}
