//
// Created by rgerst on 30.10.18.
//


#pragma once

#include <cxxh/access/cache.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <coixx/toolbox/toolbox_io.h>
#include <misaxx/descriptions/misa_image_file_description.h>
#include <misaxx/misa_cache.h>
#include <misaxx/patterns/misa_image_file_pattern.h>
#include <misaxx/patterns/misa_image_file_stack_pattern.h>
#include <misaxx/runtime/misa_runtime_base.h>
#include <misaxx/misa_default_cache.h>

namespace misaxx {
    /**
     * A standard cached 2D image file.
     * @tparam Image Image loadable by coixx::toolbox::auto_load() / coixx::toolbox::load() and saveable by coixx::toolbox::save()
     */
    template<class Image> class misa_image_file : public misa_default_cache<cxxh::access::cache<Image>, misa_image_file_pattern, misa_file_description> {
    public:

        Image &get() override {
            return m_value;
        }

        const Image &get() const override {
            return m_value;
        }

        void set(Image value) override {
            m_value = std::move(value);
            m_has_value = true;
        }

        bool has() const override {
            return m_has_value;
        }

        bool can_pull() const override {
            return boost::filesystem::is_regular_file(m_path);
        }

        void pull() override {
            m_value = coixx::toolbox::auto_load<Image>(m_path);
        }

        void stash() override {
            m_value = Image();
        }

        void push() override {
            using namespace coixx::toolbox;
            m_value << save(m_path);
            stash();
        }

        void do_link(const misa_file_description &t_description) override {
            m_path = this->get_location() / t_description.filename;
            this->set_unique_location(m_path);
        }

    protected:

        misa_file_description produce_description(const boost::filesystem::path &t_location, const misa_image_file_pattern &t_pattern) override {
            return t_pattern.produce(t_location);
        }

    private:
        Image m_value;
        bool m_has_value = false;
        boost::filesystem::path m_path;
    };
}
