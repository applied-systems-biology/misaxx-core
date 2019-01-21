//
// Created by rgerst on 18.01.19.
//


#pragma once

#include <string>
#include <boost/lexical_cast.hpp>

namespace misaxx::utils {
    /**
     * Converts the input into a string similar to using std::to_string or a stream buffer.
     * Shortcut for boost::lexical_cast<std::string>
     *
     * @tparam Source
     * @param t_src
     * @return
     */
    template<class Source> inline std::string to_string(const Source &t_src) {
        return boost::lexical_cast<std::string>(t_src);
    }
}
