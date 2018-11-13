//
// Created by rgerst on 13.11.18.
//


#pragma once

#include <misaxx/runtime/misa_runtime_base.h>
#include "misa_cache.h"

namespace misaxx {
    /**
     * Common implementation of misa_cache that automates lots of required functions
     */
    template<class Cache> class misa_default_cache : public Cache, public misa_cache {
    public:
        misa_default_cache() = default;

        /**
         * Called when a link is simulated.
         * Just call describe()->access<PATTERN>() and describe()->access<DESCRIPTION>() in this function
         */
        virtual void simulate_link() = 0;

        /**
         * Applies the actual link operation
         */
        virtual void do_link() = 0;

        void link(const boost::filesystem::path &t_location, const std::shared_ptr<misa_description_storage> &t_description) final {
            m_description = t_description;
            m_location = t_location;

            if(misa_runtime_base::instance().is_simulating()) {
                simulate_link();
                return;
            }

            do_link();
        }

        std::shared_ptr<misa_description_storage> describe() const override {
            return m_description;
        }

        boost::filesystem::path get_location() const override {
            return m_location;
        }

        boost::filesystem::path get_unique_location() const override {
            return m_unique_location;
        }

        boost::filesystem::path set_unique_location(boost::filesystem::path t_path) {
            m_unique_location = std::move(t_path);
        }

    private:
        std::shared_ptr<misa_description_storage> m_description;
        boost::filesystem::path m_location;
        boost::filesystem::path m_unique_location;
    };
}
