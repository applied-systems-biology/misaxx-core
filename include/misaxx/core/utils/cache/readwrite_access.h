//
// Created by rgerst on 26.10.18.
//


#pragma once

#include <mutex>
#include <shared_mutex>
#include <misaxx/core/utils/cache/cache.h>

namespace misaxx::utils {
    /**
     * Cache access that reads the value from the cache.
     * If the cache instance has no ability to pull the data, an exception is thrown.
     * @tparam Cache
     */
    template<typename Value> struct readwrite_access {
    public:
        using value_type = Value;

        explicit readwrite_access(cache<Value> &t_cache) : m_cache(&t_cache), m_lock(t_cache.exclusive_access()) {
            m_lock.lock();
            m_cache->pull();
        }

        ~readwrite_access() {
            m_cache->push(); // Push back into the cache
        }

        readwrite_access(const readwrite_access<Value> &src) = delete;

        readwrite_access(readwrite_access<Value> &&src) noexcept = default;

        const value_type &get() const {
            return m_cache->get();
        }

        value_type &get() {
            return m_cache->get();
        }

        const value_type &operator ->() const {
            return m_cache->get();
        }

        value_type &operator ->() {
            return m_cache->get();
        }

        void set(value_type &&t_value) {
            m_cache->set(std::forward<value_type >(t_value));
        }

    private:
        cache<Value> *m_cache;
        std::unique_lock<std::shared_mutex> m_lock;
    };
}
