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
     * A cache that holds an OpenCV cv::Mat or a coixx::image
     * @tparam Image
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
            if constexpr (std::is_same<cv::Mat, Image>::value) {
                // Load it as cv::Mat
                m_value = cv::imread(m_path.string(), cv::IMREAD_UNCHANGED);
            }
            else {
                // Assume it's a coixx::image
                m_value = coixx::toolbox::auto_load<Image>(m_path);
            }
        }

        void stash() override {
            m_value = Image();
        }

        void push() override {
            if constexpr (std::is_same<cv::Mat, Image>::value) {
                cv::imwrite(m_path.string(), m_value);
            }
            else {
                using namespace coixx::toolbox;
                m_value << save(m_path);
            }
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
