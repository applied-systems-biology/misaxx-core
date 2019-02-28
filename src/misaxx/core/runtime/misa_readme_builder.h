//
// Created by rgerst on 28.02.19.
//

#pragma once

#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>

namespace misaxx {

    extern void build_readme(const nlohmann::json &schema, const boost::filesystem::path &t_output_path);

}