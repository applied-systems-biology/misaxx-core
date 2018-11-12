//
// Created by rgerst on 09.11.18.
//


#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace misaxx {

    /**
     * Well defined way to communicate the origin of serialization
     */
    struct misa_serialization_id {

        explicit misa_serialization_id() : id("misa:unknown") {

        }

        explicit misa_serialization_id(const std::string &t_module, const std::string &t_path) : id(t_module + ":" + t_path) {

        }

        explicit misa_serialization_id(std::string t_id) : id(std::move(t_id)) {
            auto colon_it = id.find(':');
            if(colon_it == std::string::npos)
                throw std::runtime_error("Invalid ID " + id);
        }

        explicit operator std::string() const {
            return id;
        }

        std::string get_module() const {
            auto colon_it = id.find(':');
            if(colon_it == std::string::npos)
                throw std::runtime_error("Invalid ID " + id);
            return id.substr(colon_it);
        }

        std::string get_path() const {
            auto colon_it = id.find(':');
            if(colon_it == std::string::npos)
                throw std::runtime_error("Invalid ID " + id);
            return id.substr(0, colon_it - 1);
        }

        const std::string &get_id() const {
            return id;
        }

    private:

        std::string id;
    };

    void to_json(nlohmann::json& j, const misa_serialization_id& p) {
        j = p.get_id();
    }

    void from_json(const nlohmann::json& j, misa_serialization_id& p) {
        p = misa_serialization_id(j.get<std::string>());
    }

}
