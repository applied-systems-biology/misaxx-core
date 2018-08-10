//
// Created by rgerst on 08.08.18.
//


#pragma once

#include "misaxx/filesystem/virtual_filesystem.h"
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

namespace misaxx::filesystem {
    using namespace nlohmann;

    void to_json(json &j, const vfs_file &p) {
        j = p.external_path().string();
    }

    void from_json(const json &j, vfs_file &p) {
        p.custom_external = j.get<std::string>();
    }

    void to_json(json &j, const vfs_folder &p) {

    }

    void from_json(const json &j, vfs_folder &p) {
        for (auto it = j.begin(); it != j.end(); ++it) {
            if (it.value().is_object()) {
                auto loaded = it.value().get<vfs_folder>();
                loaded.name = it.key();
                p.insert(std::make_shared<vfs_folder>(std::move(loaded)));
            }
            else if(it.value().is_string()) {
                auto loaded = it.value().get<vfs_file>();
                loaded.name = it.key();
                p.insert(std::make_shared<vfs_file>(std::move(loaded)));
            }
        }
    }
}

namespace misaxx::filesystem::importers {
    struct json_importer {

        boost::filesystem::path json_file;

        virtual_filesystem import() {

            std::ifstream stream;
            stream.open(json_file.string());
            nlohmann::json j;
            stream >> j;

            return j.get<virtual_filesystem>();
        }
    };
}