//
// Created by rgerst on 26.10.18.
//


#pragma once

#include <mutex>
#include <shared_mutex>
#include "cache.h"

namespace misaxx::utils {
    /**
     * Cache access that only writes data to the cache.
     * @tparam Cache
     */
    template<typename Value> struct write_access {
    public:
        using value_type = Value;

        explicit write_access(cache<Value> &t_cache) : m_cache(&t_cache), m_lock(t_cache.exclusive_access()) {
            m_lock.lock();
        }

        ~write_access() {
            m_cache->push(); // Push into the cache
        }

        write_access(const write_access<Value> &src) = delete;

        write_access(write_access<Value> &&src) noexcept = default;

        void set(value_type t_value) {
            m_cache->set(std::move(t_value));
        }

    private:
        cache<Value> *m_cache;
        std::unique_lock<std::shared_mutex> m_lock;
    };
}
