//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <misaxx/misa_default_cache.h>
#include <misaxx/descriptions/misa_file_description.h>
#include <misaxx/patterns/misa_file_pattern.h>
#include <misaxx-helpers/cache.h>

namespace misaxx {

    /**
     * Cache that stores its attachments in a JSON file.
     * The unique location
     */
    struct misa_exported_attachments_cache : public misa_default_cache<cxxh::cache<nlohmann::json>, misa_file_pattern, misa_file_description> {

        nlohmann::json &get() override;

        const nlohmann::json &get() const override;

        void set(nlohmann::json value) override;

        bool has() const override;

        bool can_pull() const override;

        void pull() override;

        void stash() override;

        void push() override;

        void do_link(const misa_file_description &t_description) override;

        /**
         * Saves the metadata included in this instance to the target JSON file
         */
        void save_attachments();

        void postprocess() override;

        void simulate_link() override;

    protected:

        misa_file_description
        produce_description(const boost::filesystem::path &, const misa_file_pattern &) override;

    private:

        boost::filesystem::path m_path;
        nlohmann::json m_json = nlohmann::json::object();
    };
}