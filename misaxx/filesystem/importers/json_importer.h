//
// Created by rgerst on 08.08.18.
//


#pragma once

#include "../misa_filesystem.h"
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

namespace misaxx::filesystem {
    using namespace nlohmann;

    void to_json(json &j, const misa_stack &p) {
        j["files"] = p.files;
        j["type"] = "stack";
    }

    void from_json(const json &j, misa_stack &p) {
        p.files = j["files"].get<misa_stack::paths_t>();
    }

    void to_json(json &j, const misa_folder &p) {

    }

    void from_json(const json &j, misa_folder &p) {
        for (auto it = j.begin(); it != j.end(); ++it) {
            if (it.value().is_object()) {
                if(it.value()["type"] == "folder") {
                    auto loaded = it.value().get<misa_folder>();
                    loaded.name = it.key();
                    p.insert(std::make_unique<misa_folder>(std::move(loaded)));
                }
                else if(it.value()["type"] == "stack") {
                    auto loaded = it.value().get<misa_stack>();
                    loaded.name = it.key();
                    p.insert(std::make_unique<misa_stack>(std::move(loaded)));
                }
            }
        }
    }
}

namespace misaxx::filesystem::importers {

    /**
     * Imports a filesystem from a JSON configuration that has the following form:
     * \code{.json}
     * {
     *  "my_data1" : {
     *      "type" : "data",
     *      "my_stack1" : {
     *          "type" : "stack",
     *          "files" : {
     *              "my_file_1" : "/data/file1",
     *              "my_file_2" : "/data/file2",
     *              "my_file_3" : "/data/file3"
     *          }
     *      }
     *  }
     * }
     * \endcode
     */
    struct json_importer {

        boost::filesystem::path json_file;

        misa_filesystem import() {

            std::ifstream stream;
            stream.open(json_file.string());
            nlohmann::json j;
            stream >> j;

            return j.get<misa_filesystem>();
        }
    };
}