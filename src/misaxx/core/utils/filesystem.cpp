/**
 * Copyright by Ruman Gerst
 * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
 * https://www.leibniz-hki.de/en/applied-systems-biology.html
 * HKI-Center for Systems Biology of Infection
 * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
 * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
 *
 * This code is licensed under BSD 2-Clause
 * See the LICENSE file provided with this code for the full license.
 */

#include <boost/filesystem.hpp>
#include <misaxx/core/utils/filesystem.h>

boost::filesystem::path
misaxx::utils::relativize_to_direct_parent(boost::filesystem::path t_parent, boost::filesystem::path t_path) {

//    std::cout << "PARENT: " << t_parent << " PATH: " << t_path << "\n";

    // Make all paths absolute
    if(!t_path.is_absolute())
        t_path = boost::filesystem::absolute(t_path, boost::filesystem::current_path());
    if(!t_parent.is_absolute())
        t_parent = boost::filesystem::absolute(t_parent, boost::filesystem::current_path());

//    std::cout << "PARENTMODIFIED: " << t_parent << " PATHMODIFIED: " << t_path << "\n";

    boost::filesystem::path result;
    while(t_path != t_parent) {
        if(!t_path.has_parent_path())
            return boost::filesystem::path();
        result = t_path.filename() / result;
        t_path = t_path.parent_path();
    }

//    std::cout << "RESULT: " << result << "\n";

    return result;
}
