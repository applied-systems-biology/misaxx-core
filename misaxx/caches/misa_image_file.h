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
    template<class Image> class misa_image_file : public misa_default_cache<Image> {
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

        void simulate_link() override {
            this->describe()->template access<misa_file_stack_pattern>();
            this->describe()->template access<misa_image_file_description>();
        }

        void do_link() override {
            if(!this->describe()->has_description()) {
                this->describe()->set(this->describe()->template get<misa_image_file_stack_pattern>().produce(this->get_location()));
            }
            m_path = this->get_location() / this->describe()->template get<misa_image_file_description>().filename;
            this->set_unique_location(m_path);
        }

    private:
        Image m_value;
        bool m_has_value = false;
        boost::filesystem::path m_path;
    };
}
