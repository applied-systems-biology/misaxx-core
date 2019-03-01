//
// Created by rgerst on 17.01.19.
//

#pragma once
#include <misaxx/core/misa_module_info.h>

namespace misaxx {
    /**
     * Allows creating a new misa_module_info
     */
    struct misa_mutable_module_info : public misa_module_info {

        misa_mutable_module_info() = default;

        void set_id(std::string t_id);

        void set_name(std::string t_name);

        void set_description(std::string t_description);

        void set_version(std::string t_version);

        void add_dependency(misa_module_info t_dependency);
      
        void set_url(std::string t_url);
        
        void add_author(std::string t_author);

        void set_authors(std::vector<std::string> t_authors);
        
        void set_organization(std::string t_organization);

        void set_license(std::string t_license);

        void set_citation(std::string t_citation);

        void set_is_external(bool t_flag);

        operator misa_module_info();

    };

}



