//
// Created by rgerst on 14.01.19.
//

#pragma once


#include <misaxx/core/misa_dispatcher_blueprint_list.h>

namespace misaxx {
    /**
     * Allows instantiation of a specific subtree supported by a misa_dispatcher_blueprint
     */
    struct misa_dispatcher_builder : public misa_dispatcher_blueprint_list {
        using misa_dispatcher_blueprint_list::misa_dispatcher_blueprint_list;

        /**
         * Instantiates a blueprint
         * @tparam Instance
         * @param t_key
         * @return
         */
        template<class Instance> Instance &build(const std::string &t_key) const {
            return dynamic_cast<Instance&>(this->get(t_key)->dispatch(this->get_worker()));
        }
    };
}

