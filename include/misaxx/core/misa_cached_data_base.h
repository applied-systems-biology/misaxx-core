//
// Created by rgerst on 13.11.18.
//

#pragma once

#include <misaxx/core/utils/dynamic_singleton_map.h>
#include <misaxx/core/utils/cache.h>
#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/misa_description_storage.h>
#include <misaxx/core/misa_cache.h>

namespace misaxx {

    template<typename Value> using readonly_access = misaxx::utils::readonly_access<Value>;
    template<typename Value> using readwrite_access = misaxx::utils::readwrite_access<Value>;
    template<typename Value> using write_access = misaxx::utils::write_access<Value>;

    /**
     * Base class for misaxx::misa_cached_data
     */
    struct misa_cached_data_base {
        using attachment_type = misaxx::utils::dynamic_singleton_map<misa_serializable>;
        using attachment_cache_type = misaxx::utils::memory_cache<attachment_type>;

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
        * @param autofill_location If true, set the location of a misa_locatable attachment to the location interface of this cache if the location is not set
        */
        template<class Attachment> void attach(Attachment attachment, bool autofill_location = true) {
            if constexpr (std::is_base_of<misa_locatable, Attachment>::value) {
                if(!attachment.template has_location<misa_location>()) {
                    attachment.set_location(get_location_interface());
                }
            }
            readwrite_access <attachment_type > access(get_cache_base()->attachments);
            access.get().insert(std::move(attachment));
        }

        /**
         * Attaches data to this cache.
         * If the attachment is a misa_locatable, set it to the location of the foreign cache (if not already set)
         * @tparam Attachment
         * @tparam CacheInterface
         * @param attachment
         * @param cache
         */
        template<class Attachment, class CacheInterface>
        void attach_foreign(Attachment attachment, const CacheInterface &cache) {
            if constexpr (std::is_base_of<misa_locatable, Attachment>::value) {
                if(!attachment.template has_location<misa_location>()) {
                    attachment.set_location(cache.get_location_interface());
                }
            }
            attach<Attachment>(std::move(attachment));
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
        virtual std::shared_ptr<misa_description_storage> describe() const = 0;

        /**
         * Returns true if this cached data has an actual cache assigned to it
         * @return
         */
        virtual bool has_data() const = 0;

        /**
         * Returns the location of the cache within the internal file system
         * This is the folder that contains the data. Please note that
         * this location might not be unique for different caches, as multiple caches might be created
         * on the same location.
         * Use get_unique_location() to find the actual file instead.
         * @return
         */
        virtual boost::filesystem::path get_internal_location() const = 0;

        /**
         * Returns the location of the cache.
         * This is the folder that contains the data. Please note that
         * this location might not be unique for different caches, as multiple caches might be created
         * on the same location.
         * Use get_unique_location() to find the actual file instead.
         * @return
         */
        virtual boost::filesystem::path get_location() const = 0;

        /**
         * Returns the unique location of the cache.
         * This usually points to the actual file containing the data
         * @return
         */
        virtual boost::filesystem::path get_unique_location() const = 0;

        /**
         * Gets the location interface of this cache. This location interface is compatible with
         * misaxx::misa_locatable instances.
         * @return
         */
        virtual std::shared_ptr<const misa_location> get_location_interface() const = 0;
    };

}



