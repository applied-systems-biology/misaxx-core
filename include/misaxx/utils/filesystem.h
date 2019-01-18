//
// Created by rgerst on 18.01.19.
//

#pragma once


#include <boost/filesystem/path.hpp>

namespace misaxx::utils {

    /**
    * Returns the relative path of the input path to its direct parent
    * This differs from Boost's relative() function by not returning a path if the parent is not a direct parent
    * @param t_parent
    * @param t_path
    * @return
    */
    extern boost::filesystem::path relativize_to_direct_parent(boost::filesystem::path t_parent, boost::filesystem::path t_path);

}


