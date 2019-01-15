//
// Created by rgerst on 14.01.19.
//

#pragma once


#include "misa_dispatcher_blueprints.h"

namespace misaxx {
    /**
     * Allows instantiation of a specific subtree supported by a misa_dispatcher_blueprint
     */
    struct misa_dispatcher_builder : public misa_dispatcher_blueprints {
        using misa_dispatcher_blueprints::misa_dispatcher_blueprints;

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

