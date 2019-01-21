//
// Created by rgerst on 18.01.19.
//

#include <boost/filesystem.hpp>
#include <misaxx/core/utils/filesystem.h>

boost::filesystem::path
misaxx::utils::relativize_to_direct_parent(boost::filesystem::path t_parent, boost::filesystem::path t_path) {

//    std::cout << "PARENT: " << t_parent << " PATH: " << t_path << std::endl;

    // Make all paths absolute
    if(!t_path.is_absolute())
        t_path = boost::filesystem::absolute(t_path, boost::filesystem::current_path());
    if(!t_parent.is_absolute())
        t_parent = boost::filesystem::absolute(t_parent, boost::filesystem::current_path());

//    std::cout << "PARENTMODIFIED: " << t_parent << " PATHMODIFIED: " << t_path << std::endl;

    boost::filesystem::path result;
    while(t_path != t_parent) {
        if(!t_path.has_parent_path())
            return boost::filesystem::path();
        result = t_path.filename() / result;
        t_path = t_path.parent_path();
    }

//    std::cout << "RESULT: " << result << std::endl;

    return result;
}
