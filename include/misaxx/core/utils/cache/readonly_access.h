//
// Created by rgerst on 26.10.18.
//


#pragma once

#include <mutex>
#include <shared_mutex>
#include <misaxx/core/utils/cache/cache.h>

namespace misaxx::utils {
    template<typename Value>
    class readonly_access {
    public:
        using value_type = Value;

        explicit readonly_access(cache<Value> &t_cache) : m_cache(&t_cache), m_lock(t_cache.readonly_access()) {
            m_lock.lock();
            m_cache->pull();
        }

        ~readonly_access() {
            m_cache->try_stash(); // Push back into the cache
        }

        readonly_access(const readonly_access<Value> &src) = delete;

        readonly_access(readonly_access<Value> &&src) noexcept = default;

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

    private:
        cache<Value> *m_cache;
        std::shared_lock<std::shared_mutex> m_lock;
    };
}
