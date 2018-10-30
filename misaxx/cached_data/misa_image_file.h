//
// Created by rgerst on 30.10.18.
//


#pragma once

#include <cxxh/access/cache.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <coixx/toolbox/toolbox_io.h>
#include "misa_cache.h"

namespace misaxx {
    /**
     * A standard cached 2D image file.
     * @tparam Image Image loadable by coixx::toolbox::auto_load() / coixx::toolbox::load() and saveable by coixx::toolbox::save()
     */
    template<class Image> class misa_image_file : public cxxh::access::cache<Image>, public misa_cache {
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

        void link(const filesystem::const_entry &t_location) override {

        }

        void create(const filesystem::const_entry &t_location, const misa_filesystem_metadata &t_description) override {

        }

        misa_filesystem_metadata describe() override {
            return misa_filesystem_metadata();
        }

    private:
        Image m_value;
        bool m_has_value;
        boost::filesystem::path m_path;

    };
}
