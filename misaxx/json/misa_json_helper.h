//
// Created by rgerst on 12.11.18.
//


#pragma once

#include <nlohmann/json.hpp>

namespace misaxx::json_helper {

    /**
     * Accesses a JSON path
     * @param t_json
     * @param t_path
     * @return
     */
    inline nlohmann::json &access_json_path(nlohmann::json &t_json, const std::vector<std::string> &t_path) {
        nlohmann::json *current = &t_json;
        for(const auto &segment : t_path) {
            current = &(*current)[segment];
        }
        return *current;
    }

    template<class ...Args> inline nlohmann::json &access_json_path(nlohmann::json &t_json, const std::vector<std::string> &t_path, const std::string &t_after, const Args&... t_args) {
        nlohmann::json *current = &t_json;
        for(const auto &segment : t_path) {
            current = &(*current)[segment];
        }
        current = &(*current)[t_after];
        for(const std::string &segment : std::initializer_list<std::string>({ t_args... })) {
            current = &(*current)[segment];
        }
        return *current;
    }

}