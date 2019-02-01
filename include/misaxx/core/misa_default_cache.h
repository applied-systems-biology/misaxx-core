//
// Created by rgerst on 13.11.18.
//


#pragma once

#include <misaxx/core/misa_cache.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>

namespace misaxx {

    /**
     * Automates some implementation details required by misa_cache
     * @tparam Cache The misaxx::utils::access::cache implementation
     * @tparam Pattern The pattern that this cache is is using to create a pattern
     * @tparam Description Description type of this cache
     */
    template<class Cache, class Pattern, class Description> class misa_default_cache : public Cache, public misa_cache {
    public:

        using default_cache_type = misa_default_cache<Cache, Pattern, Description>;
        using cache_type = Cache;
        using pattern_type = Pattern;
        using description_type = Description;

        misa_default_cache() = default;

        /**
         * Called when a link is simulated.
         * Just call describe()->access<PATTERN>() and describe()->access<DESCRIPTION>() in this function
         */
        virtual void simulate_link() {
            m_description->access<Pattern>();
            m_description->access<Description>();
        }


        /**
         * Applies the actual link operation
         * @param t_description same as describe()->get<Description>()
         */
        virtual void do_link(const Description &t_description) = 0;

        void link(const boost::filesystem::path &t_location, const std::shared_ptr<misa_description_storage> &t_description) final {
            m_description = t_description;
            m_location = t_location;

            if(misaxx::runtime_properties::is_simulating()) {
                simulate_link();
                return;
            }

            // Create the description if needed
            if(!m_description->has_description()) {
                m_description->set(produce_description(m_location, m_description->access<Pattern>()));
            }

            do_link(m_description->get<Description>());
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

        void set_unique_location(boost::filesystem::path t_path) {
            m_unique_location = std::move(t_path);
        }

        bool has_data() override {
            return this->has();
        }

        std::shared_ptr<const misa_location> get_location_interface() const override {
            if(!static_cast<bool>(m_location_interface)) {
                m_location_interface = create_location_interface();
            }
            return m_location_interface;
        }

    protected:

        /**
         * Instantiates a description from the underlying pattern
         * @param t_location same as get_location()
         * @param t_pattern same as describe()->get<Pattern>()
         * @return
         */
        virtual Description produce_description(const boost::filesystem::path &t_location, const Pattern &t_pattern) = 0;

        /**
         * Creates a location interface for this cache
         * Override this behavior for more detailed location interfaces
         * @return
         */
        virtual std::shared_ptr<misa_location> create_location_interface() const {
            auto interface = std::make_shared<misa_location>();
            interface->filesystem_location = get_location();
            interface->filesystem_unique_location = get_unique_location();
            return interface;
        }

    private:
        std::shared_ptr<misa_description_storage> m_description;
        boost::filesystem::path m_location;
        boost::filesystem::path m_unique_location;
        mutable std::shared_ptr<misa_location> m_location_interface;
    };
}