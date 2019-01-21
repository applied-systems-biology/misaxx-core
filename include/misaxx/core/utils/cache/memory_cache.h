//
// Created by rgerst on 29.10.18.
//


#pragma once

#include <misaxx/core/utils/cache/cache.h>

namespace misaxx::utils {
    /**
     * Convenience class that caches a value in memory, but allows thread-safe interaction
     * @tparam Value
     */
    template<typename Value> class memory_cache : public cache<Value> {
    public:

        memory_cache() = default;

        explicit memory_cache(Value value) : m_value(std::move(value)) {

        }

        Value &get() override {
            return m_value;
        }

        const Value &get() const override {
            return m_value;
        }

        void set(Value value) override {
            m_value = std::move(value);
        }

        bool has() const override {
            return true;
        }

        bool can_pull() const override {
            return true;
        }

        void pull() override {

        }

        void stash() override {

        }

        void push() override {

        }
    private:
        Value m_value;
    };
}
