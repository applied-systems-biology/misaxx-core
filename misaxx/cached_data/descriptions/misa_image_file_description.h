//
// Created by rgerst on 30.10.18.
//


#pragma once

#include "misa_file_description.h"
#include <coixx/opencv_image.h>

namespace misaxx {
    struct misa_image_file_description : public misa_file_description {

        /**
         * How many bits are allocated per channel
         */
        int bits_per_channel;

        /**
         * Either S (signed), U (unsigned) or F (floating point)
         */
        char bits_description;

        /**
         * Number of channels
         */
        int channels;

        /**
         * How channels are interpreted
         */
        std::string channel_allocation;

        misa_image_file_description() = default;

        /**
         *
         * @param t_filename Filename
         * @param t_filetype Extension of the file (e.g. .tif)
         * @param t_image_type String that explains what the image contains
         */
        explicit misa_image_file_description(std::string t_filename, std::string t_filetype, const std::string &t_image_type) :
                misa_file_description(std::move(t_filename), std::move(t_filetype)) {
            set_image_type(t_filetype);
        }

        void from_json(const nlohmann::json &t_json) override {
            misa_file_description::from_json(t_json);
            if(t_json.find("image-type") != t_json.end()) {
                set_image_type(t_json["image-type"]);
            }
            else {
                bits_per_channel = t_json["bits-per-channel"];
                bits_description = t_json["channel-bit-type"].get<std::string>().at(0);
                channels = t_json["num-channels"];
                channel_allocation = t_json["channel-allocation"];
            }
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_file_description::to_json(t_json);
            t_json["bits-per-channel"] = bits_per_channel;
            t_json["channel-bit-type"] = bits_description;
            t_json["num-channels"] = channels;
            t_json["channel-allocation"] = channel_allocation;
        }

        std::string get_name() const override {
            return "misa-image-file";
        }

        void set_image_type(const std::string &t_image_type) {
            std::stringstream ss(t_image_type);
            char C;
            char Us;
            ss >> bits_per_channel >> bits_description >> C >> channels >> Us >> channel_allocation;
        }

        std::string get_image_type() {
            std::stringstream stringstream;
            stringstream << bits_per_channel << bits_description << "C" << channels << "_" << channel_allocation;
            return stringstream.str();
        }

    };
}
