//
// Created by rgerst on 01.11.18.
//


#pragma once

#include <misaxx/filesystem/misa_filesystem_entry.h>
#include <misaxx/misa_serializeable.h>

namespace misaxx {

    struct misa_pattern_base {
    };

    /**
     * A pattern consists of externally provided information how to import data.
     * During the import process, the pattern is analyzed and transformed into a description, which contains
     * all necessary data for additional processing.
     *
     * It should have a method Description produce(...) const that produces a description based on the parameters.
     *
     * @tparam Description
     */
    template<class Description> struct misa_data_pattern : public misa_serializeable, public misa_pattern_base {

        using description_type = Description;

        /**
         * Internally used by the metadata holder to serialize this element.
         * @return
         */
        std::string get_name() const final {
            return "patterns";
        }
    };
}
