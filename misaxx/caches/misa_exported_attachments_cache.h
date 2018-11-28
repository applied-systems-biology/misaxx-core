//
// Created by rgerst on 11.09.18.
//


#pragma once

#include <misaxx/misa_default_cache.h>
#include <misaxx/descriptions/misa_file_description.h>
#include <misaxx/patterns/misa_file_pattern.h>
#include <cxxh/cache.h>

namespace misaxx {

    /**
     * Cache that stores its attachments in a JSON file.
     * The unique location
     */
    struct misa_exported_attachments_cache : public misa_default_cache<cxxh::cache<nlohmann::json>, misa_file_pattern, misa_file_description> {

        nlohmann::json &get() override {
            return m_json;
        }

        const nlohmann::json &get() const override {
            return m_json;
        }

        void set(nlohmann::json value) override {
            m_json = std::move(value);
        }

        bool has() const override {
            return false;
        }

        bool can_pull() const override {
            return boost::filesystem::is_regular_file(m_path);
        }

        void pull() override {
            std::ifstream sw;
            sw.open(m_path.string());
            sw >> m_json;
        }

        void stash() override {
            m_json = nlohmann::json::object();
        }

        void push() override {
            std::ofstream sw;
            sw.open(m_path.string());
            sw << std::setw(4) << m_json;
        }

        void do_link(const misa_file_description &t_description) override {
            m_path = this->get_location() / t_description.filename;
            this->set_unique_location(m_path);
        }

        /**
         * Saves the metadata included in this instance to the target JSON file
         */
        void save_attachments() {
            cxxh::readonly_access<attachment_type > access(attachments);
            for(const auto &kv : access.get()) {
                const misa_serializeable *md = kv.second.get();
                md->to_json(m_json[md->get_serialization_id().get_id()]);
            }
        }

        void postprocess() override {
            save_attachments();
            push();
        }

        void simulate_link() override {
            describe()->set(misa_file_pattern({ ".json" }));
            describe()->access<misa_file_description>();
        }

    protected:

        misa_file_description
        produce_description(const boost::filesystem::path &t_location, const misa_file_pattern &t_pattern) override {
            return misa_file_description();
        }

    private:

        boost::filesystem::path m_path;
        nlohmann::json m_json = nlohmann::json::object();
    };
}