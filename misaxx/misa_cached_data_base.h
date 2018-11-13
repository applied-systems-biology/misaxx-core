//
// Created by rgerst on 13.11.18.
//

#pragma once

#include <cxxh/containers/dynamic_singleton_map.h>
#include <cxxh/access/memory_cache.h>
#include <cxxh/access/readonly_access.h>
#include <cxxh/access/readwrite_access.h>
#include <cxxh/access/write_access.h>
#include <cxxh/access/memory_cache.h>
#include "misa_serializeable.h"
#include "misa_description_storage.h"
#include "misa_cache.h"

namespace misaxx {

    template<typename Value> using readonly_access = cxxh::access::readonly_access<Value>;
    template<typename Value> using readwrite_access = cxxh::access::readwrite_access<Value>;
    template<typename Value> using write_access = cxxh::access::write_access<Value>;

    /**
     * Base class for cached data
     */
    struct misa_cached_data_base {
        using attachment_type = cxxh::containers::dynamic_singleton_map<misa_serializeable>;
        using attachment_cache_type = cxxh::access::memory_cache<attachment_type>;

        misa_cached_data_base() = default;

        /**
         * Returns the cache stored within this cache storage
         * @return
         */
        virtual std::shared_ptr<misa_cache> get_cache_base() const = 0;

        /**
        * Attaches data to this cache
        * @tparam Attachment
        * @param attachment
        */
        template<class Attachment> void attach(Attachment attachment) {
            readwrite_access <attachment_type > access(get_cache_base()->attachments);
            access.get().insert(std::move(attachment));
        }

        /**
         * Gets copy of attached data.
         * Throws exception if attachment does not exist
         * @tparam Attachment
         * @return
         */
        template<class Attachment> Attachment get_attachment() const {
            readwrite_access <attachment_type > access(get_cache_base()->attachments);
            return access.get().at<Attachment>();
        }

        /**
        * Gets copy of attached data.
        * Throws exception if attachment does not exist
        * @tparam Attachment
        * @return
        */
        template<class Attachment> bool has_attachment() const {
            readonly_access <attachment_type > access(get_cache_base()->attachments);
            return access.get().has<Attachment>();
        }

        /**
        * Returns true if this cache is set from a parent module
        * @return
        */
        virtual bool is_externally_set() const = 0;

        /**
        * Returns a description of the current cache
        * @return
        */
        virtual std::shared_ptr<misa_description_storage> describe() = 0;

        /**
         * Returns true if this cached data has an actual cache assigned to it
         * @return
         */
        virtual bool has_cache() const = 0;
    };

}



